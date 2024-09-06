/*
 * Encoder.c
 *
 *  Created on: Aug 29, 2024
 *      Author: Sam
 */

#include "Generic/Encoder.h"
#include "Generic/Activation.h"
#include "Display/OLEDCtrl.h"
#include "TestFunc/TestFunc.h"

typedef struct
{
	bool isRisingEdge;
	bool isFallingEdge;
	uint32_t tickRising;
	uint32_t tickFalling;
	uint32_t tickTimeOut;
}EXTICallbackParam_t;

static EXTICallbackParam_t ExCbkParam =
{
	.isRisingEdge = false,
	.isFallingEdge = false,
	.tickRising = 0,
	.tickFalling = 0,
	.tickTimeOut = 500, // 500ms time out
};

static EncoderParam_t EncParam =
{
	.isCountUp = false,
	.isResistorConv = false,
	.cntEC1 = 0,
	.cntEC2 = 0,
	.cntIndex = 0,
	.cntRawValue = 0,
	.cntRawValueTmp = 0,
	.cntMultipleBig = 0,
	.cntMultipleSmall = 0,
};

static void ShowEncoderCount(void)
{
	uint16_t cntRawTmp = EncParam.cntRawValue * 50;
	char strCount[10];

	OLED_Clear(0);
	if(EncParam.isResistorConv)
	{
		// 1 raw value equals 50 ohms
		sprintf(strCount, "%d", cntRawTmp);
		if(cntRawTmp >= 10000)
			OLED_ShowString_11x18W(6, 0, strCount);
		else if(cntRawTmp >= 1000)
			OLED_ShowString_11x18W(15, 0, strCount);
		else if(cntRawTmp >= 100)
			OLED_ShowString_11x18W(26, 0, strCount);
		else
			OLED_ShowString_11x18W(37, 0, strCount);
		OLED_ShowString_11x18W(61, 0, "(Ohm)");
		sprintf(strCount, "%d", EncParam.cntRawValue);
		OLED_ShowString_7x10W(16, 22, "RawData:");
		OLED_ShowString_7x10W(72, 22, strCount);
	}
	else
	{
		sprintf(strCount, "%d", EncParam.cntRawValue);
		OLED_ShowString_11x18W(0, 0, "Relay:");
		OLED_ShowString_11x18W(68, 0, strCount);
		sprintf(strCount, "%d", EncParam.cntEC1);
		OLED_ShowString_7x10W(2, 22, "E1:");
		OLED_ShowString_7x10W(23, 22, strCount);
		sprintf(strCount, "%d", EncParam.cntEC2);
		OLED_ShowString_7x10W(58, 22, ",E2:");
		OLED_ShowString_7x10W(86, 22, strCount);
	}
}

static void ConvertRelayState(uint16_t rawVal)
{
	if(EncParam.isResistorConv)
	{
		WriteRelay(RelayOpen, 1);
		if(rawVal > 0x1FF)
		{
			rawVal = 0x1FF;
		}
		else
		{
			rawVal = 0x1FF - rawVal;
		}

	}
	else
	{
		if(rawVal != 0x00)
		{
			WriteRelay(RelayOpen, 1);
		}
		else
		{
			WriteRelay(RelayOpen, 0);
		}
	}

	for(int i = 0; i < 9; i++)
	{
		if((rawVal >> i) & 0x001)
		{
			WriteRelay(Relay9 - i, 1);
		}
		else
		{
			WriteRelay(Relay9 - i, 0);
		}
	}
}

static void ConvertLedState(uint8_t rawVal)
{
	if(rawVal & 0x10)
		WriteLED(LED3, 1);
	else
		WriteLED(LED3, 0);
	if(rawVal & 0x08)
		WriteLED(LED4, 1);
	else
		WriteLED(LED4, 0);
	if(rawVal & 0x04)
		WriteLED(LED5, 1);
	else
		WriteLED(LED5, 0);
	if(rawVal & 0x02)
		WriteLED(LED2, 1);
	else
		WriteLED(LED2, 0);
	if(rawVal & 0x01)
		WriteLED(LED1, 1);
	else
		WriteLED(LED1, 0);
}

static void RawValueBigChange(bool isCntUp)
{
	uint16_t arrRelayState[10] = { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0x1FF };

	if(EncParam.isResistorConv)
	{
		if(isCntUp)
		{
			// 1 raw value equals 50 ohms, 20*50 = 1000 ohms
			EncParam.cntRawValue += 20 * (EncParam.cntMultipleBig + 1);
			if(EncParam.cntRawValue > 0x1FF)
			{
				EncParam.cntRawValue = 0x1FF;
			}
		}
		else
		{
			EncParam.cntRawValue -= 20 * (EncParam.cntMultipleBig + 1);
			if(EncParam.cntRawValue > 0x1FF)
			{
				EncParam.cntRawValue = 0;
			}
		}
	}
	else
	{
		for(int i = 9; i >= 0; i--)
		{
			if(EncParam.cntRawValue >= arrRelayState[i])
			{
				EncParam.cntIndex = i;
				break;
			}
		}
		if(isCntUp)
		{
			EncParam.cntIndex++;
			if(EncParam.cntIndex > 9)
			{
				EncParam.cntIndex = 9;
			}
		}
		else
		{
			if(EncParam.cntIndex > 0 && EncParam.cntIndex <= 9)
			{
				EncParam.cntIndex--;
			}
			else
			{
				EncParam.cntIndex = 0;
			}
		}
		EncParam.cntRawValue = arrRelayState[EncParam.cntIndex];
	}
	ConvertLedState(EncParam.cntRawValue);
	ConvertRelayState(EncParam.cntRawValue);
}

static void RawValueSmallChange(bool isCntUp)
{
	if(isCntUp)
	{
		// EC1 will jump by 2 values ​​when it rotates one cell, so divide by 2.
		EncParam.cntRawValueTmp++;
		if(EncParam.cntRawValueTmp % 2 == 1)
		{
			EncParam.cntRawValue++;
			if(EncParam.cntMultipleSmall > 0)
			{
				EncParam.cntRawValue += (EncParam.cntMultipleSmall);
			}
			if(EncParam.cntRawValue > 0x1FF) // After addition > 0x1FF
			{
				EncParam.cntRawValue = 0x1FF;
			}
		}
	}
	else
	{
		// EC1 will jump by 2 values ​​when it rotates one cell, so divide by 2.
		EncParam.cntRawValueTmp--;
		if(EncParam.cntRawValueTmp % 2 == 1)
		{
			EncParam.cntRawValue--;
			if(EncParam.cntMultipleSmall > 0)
			{
				EncParam.cntRawValue -= (EncParam.cntMultipleSmall);
			}
			if(EncParam.cntRawValue > 0x1FF) // After subtraction < 0
			{
				EncParam.cntRawValue = 0;
			}
		}
	}
	ConvertLedState(EncParam.cntRawValue);
	ConvertRelayState(EncParam.cntRawValue);
}

// Encoder External Interrupt (Rising Edge): EC22=PD0, EC21=PD1, Ec12=PD2, Ec11=PD3
void HAL_GPIO_EXTI_Rising_Callback(uint16_t pinEc)
{
	ExCbkParam.isRisingEdge = true;
	ExCbkParam.tickRising = HAL_GetTick();

    switch(pinEc)
    {
    	case EC22_EXTI0_Pin:
    		if(!HAL_GPIO_ReadPin(EC21_EXTI1_GPIO_Port, EC21_EXTI1_Pin))
    		{
    			// Encoder: "Forward"
    			EncParam.isCountUp = true;
    			EncParam.cntEC2++;
    		}
    		break;
    	case EC21_EXTI1_Pin:
    		if(!HAL_GPIO_ReadPin(EC22_EXTI0_GPIO_Port, EC22_EXTI0_Pin))
    		{
    			// Encoder: "Reverse"
    			EncParam.isCountUp = false;
    			EncParam.cntEC2--;
    		}
    		break;
    	case EC12_EXTI2_Pin:
    		if(!HAL_GPIO_ReadPin(EC11_EXTI3_GPIO_Port, EC11_EXTI3_Pin))
    		{
    			// Encoder: "Forward"
    			EncParam.isCountUp = true;
    			EncParam.cntEC1++;
    		}
    		break;
    	case EC11_EXTI3_Pin:
    		if(!HAL_GPIO_ReadPin(EC12_EXTI2_GPIO_Port, EC12_EXTI2_Pin))
    		{
    			// Encoder: "Reverse"
    			EncParam.isCountUp = false;
    			EncParam.cntEC1--;
    		}
    		break;
    }
}

// Encoder External Interrupt (Falling Edge): EC22=PD0, EC21=PD1, Ec12=PD2, Ec11=PD3
void HAL_GPIO_EXTI_Falling_Callback(uint16_t pinEc)
{
	ExCbkParam.isFallingEdge = true;
	ExCbkParam.tickFalling = HAL_GetTick();

    switch(pinEc)
    {
    	case EC22_EXTI0_Pin:
    		if(HAL_GPIO_ReadPin(EC21_EXTI1_GPIO_Port, EC21_EXTI1_Pin))
    		{
    			// Encoder: "Forward"
    			EncParam.isCountUp = true;
    			EncParam.cntEC2++;
    		}
    		break;
    	case EC21_EXTI1_Pin:
    		if(HAL_GPIO_ReadPin(EC22_EXTI0_GPIO_Port, EC22_EXTI0_Pin))
    		{
    			// Encoder: "Reverse"
    			EncParam.isCountUp = false;
    			EncParam.cntEC2--;
    		}
    		break;
    	case EC12_EXTI2_Pin:
    		if(HAL_GPIO_ReadPin(EC11_EXTI3_GPIO_Port, EC11_EXTI3_Pin))
    		{
    			// Encoder: "Forward"
    			EncParam.isCountUp = true;
    			EncParam.cntEC1++;
    		}
    		break;
    	case EC11_EXTI3_Pin:
    		if(HAL_GPIO_ReadPin(EC12_EXTI2_GPIO_Port, EC12_EXTI2_Pin))
    		{
    			// Encoder: "Reverse"
    			EncParam.isCountUp = false;
    			EncParam.cntEC1--;
    		}
    		break;
    }
}


void ChangeEncoderBigMultiple()
{
	char strCount[6];

	if(EncParam.cntMultipleBig == 0)
	{
		EncParam.cntMultipleBig = 1;
	}
	else
	{
		EncParam.cntMultipleBig = 0;
	}
	OLED_Clear(0);
	sprintf(strCount, "%d", (EncParam.cntMultipleBig + 1) * 1000);
	OLED_ShowString_11x18W(11, 11, "Set:R*");
	OLED_ShowString_11x18W(77, 11, strCount);
}

void ChangeEncoderSmallMultiple()
{
	char strCount[6];

	if(EncParam.cntMultipleSmall == 0)
	{
		EncParam.cntMultipleSmall = 1;
	}
	else
	{
		EncParam.cntMultipleSmall = 0;
	}
	OLED_Clear(0);
	sprintf(strCount, "%d", (EncParam.cntMultipleSmall + 1) * 50);
	OLED_ShowString_11x18W(11, 11, "Set:R*");
	OLED_ShowString_11x18W(77, 11, strCount);
}

EncoderParam_t ReadEncoderParam(void)
{
	return EncParam;
}

void WriteEncoderParam(EncoderParam_t param)
{
	EncParam.cntEC1 = param.cntEC1;
	EncParam.cntEC2 = param.cntEC2;
	EncParam.cntIndex = param.cntIndex;
	EncParam.cntRawValue = param.cntRawValue;
}

void SetEncoderResistorConversion(bool isConv)
{
	EncParam.isResistorConv = isConv;
}

void SetEncoderCountRawValue(uint16_t cnt)
{
	EncParam.cntRawValue = cnt;
	EncParam.cntRawValueTmp = cnt;
}

void EncoderInit(void)
{
	ExCbkParam.isRisingEdge = false;
	ExCbkParam.isFallingEdge = false;
	EncParam.isCountUp = false;
	EncParam.cntEC1 = 0;
	EncParam.cntEC2 = 0;
	EncParam.cntIndex = 0;
	EncParam.cntRawValue = 0;
	EncParam.cntRawValueTmp = 0;
	EncParam.cntMultipleBig = 0;
	EncParam.cntMultipleSmall = 0;
}

//In this task, 2 Encoders are used to control the Relay and display the count value on OLED.
void EncoderTask(void)
{
	static uint16_t cntEC1tmp = 0;
	static uint16_t cntEC2tmp = 0;
	static uint16_t cntRVtmp = 0;

	if(ExCbkParam.isRisingEdge)
	{
		if(cntEC2tmp != EncParam.cntEC2)
		{
			cntEC2tmp = EncParam.cntEC2;
			RawValueBigChange(EncParam.isCountUp);
			SEGGER_RTT_printf(0, "-> (EC2: %d, Relay: %d)\r\n", EncParam.cntEC2, EncParam.cntRawValue);
		}
		if(cntEC1tmp != EncParam.cntEC1)
		{
			cntEC1tmp = EncParam.cntEC1;
			RawValueSmallChange(EncParam.isCountUp);
			SEGGER_RTT_printf(0, "-> (EC1: %d, Relay: %d)\r\n", EncParam.cntEC1, EncParam.cntRawValue);
		}
		if(HAL_GetTick() - ExCbkParam.tickRising > ExCbkParam.tickTimeOut)
		{
			ExCbkParam.isRisingEdge = false;
			if(cntRVtmp != EncParam.cntRawValue)
			{
				cntRVtmp = EncParam.cntRawValue;
				ShowEncoderCount();
			}
		}
	}
	else if(ExCbkParam.isFallingEdge)
	{
		if(cntEC2tmp != EncParam.cntEC2)
		{
			cntEC2tmp = EncParam.cntEC2;
			RawValueBigChange(EncParam.isCountUp);
			SEGGER_RTT_printf(0, "-> (EC2: %d, Relay: %d)\r\n", EncParam.cntEC2, EncParam.cntRawValue);
		}
		if(cntEC1tmp != EncParam.cntEC1)
		{
			cntEC1tmp = EncParam.cntEC1;
			RawValueSmallChange(EncParam.isCountUp);
			SEGGER_RTT_printf(0, "-> (EC1: %d, Relay: %d)\r\n", EncParam.cntEC1, EncParam.cntRawValue);
		}
		if(HAL_GetTick() - ExCbkParam.tickFalling > ExCbkParam.tickTimeOut)
		{
			ExCbkParam.isFallingEdge = false;
			if(cntRVtmp != EncParam.cntRawValue)
			{
				cntRVtmp = EncParam.cntRawValue;
				ShowEncoderCount();
			}
		}
	}
}
