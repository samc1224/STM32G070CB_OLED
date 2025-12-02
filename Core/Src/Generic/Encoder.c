/*
 * Encoder.c
 *
 *  Created on: Aug 29, 2024
 *      Author: Sam
 */

#include <string.h>

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
	.cntBigMultiple = 0,
	.cntSmallMultiple = 0,
};

static void ConvertLedStatus(uint8_t rawVal);

void ShowEncoderCount(void)
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

void ConvertResistorValueToRelay(uint16_t rawVal)
{
	if(EncParam.isResistorConv)
	{
		WriteRelay(RelayOpen, 1);
		if(rawVal > 0x1FF)
		{
			rawVal = 0; // Max resistor value (25550)
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

static void ConvertLedStatus(uint8_t rawVal)
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

void RawValueBigChange(bool isCntUp)
{
	uint16_t arrRelayStatus[10] = { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0x1FF };

	if(EncParam.isResistorConv)
	{
		if(isCntUp)
		{
			// EC2 will change 2 values ​​when rotating one step, so divide by 2
			EncParam.cntRawValueTmp++;
			if(EncParam.cntRawValueTmp % 2 == 1)
			{
				EncParam.cntRawValue += 20 * (EncParam.cntBigMultiple + 1); // 1 raw value = 50 ohms, 20*50 = 1000 ohms
				if(EncParam.cntRawValue > 0x1FF)
				{
					EncParam.cntRawValue = 0x1FF;
				}
			}
		}
		else
		{
			EncParam.cntRawValueTmp--;
			if(EncParam.cntRawValueTmp % 2 == 1)
			{
				EncParam.cntRawValue -= 20 * (EncParam.cntBigMultiple + 1);
				if(EncParam.cntRawValue > 0x1FF)
				{
					EncParam.cntRawValue = 0;
				}
			}
		}
	}
	else
	{
		for(int i = 9; i >= 0; i--)
		{
			if(EncParam.cntRawValue >= arrRelayStatus[i])
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
		EncParam.cntRawValue = arrRelayStatus[EncParam.cntIndex];
	}
	ConvertResistorValueToRelay(EncParam.cntRawValue);
	//ConvertLedStatus(EncParam.cntRawValue);
}

void RawValueSmallChange(bool isCntUp)
{
	if(isCntUp)
	{
		// EC1 will change 2 values ​​when rotating one step, so divide by 2
		EncParam.cntRawValueTmp++;
		if(EncParam.cntRawValueTmp % 2 == 1)
		{
			EncParam.cntRawValue++;
			if(EncParam.cntSmallMultiple > 0)
			{
				EncParam.cntRawValue += (EncParam.cntSmallMultiple);
			}
			if(EncParam.cntRawValue > 0x1FF) // After addition > 0x1FF
			{
				EncParam.cntRawValue = 0x1FF;
			}
		}
	}
	else
	{
		EncParam.cntRawValueTmp--;
		if(EncParam.cntRawValueTmp % 2 == 1)
		{
			EncParam.cntRawValue--;
			if(EncParam.cntSmallMultiple > 0)
			{
				EncParam.cntRawValue -= (EncParam.cntSmallMultiple);
			}
			if(EncParam.cntRawValue > 0x1FF) // After subtraction < 0
			{
				EncParam.cntRawValue = 0;
			}
		}
	}
	ConvertResistorValueToRelay(EncParam.cntRawValue);
	//ConvertLedStatus(EncParam.cntRawValue);
}

void ChangeEncoderBigMultiple(uint8_t cntBig)
{
	char strCount[6];

	if(cntBig >= 1)
	{
		EncParam.cntBigMultiple = 1;
	}
	else
	{
		EncParam.cntBigMultiple = 0;
	}
	OLED_Clear(0);
	sprintf(strCount, "%d", (EncParam.cntBigMultiple + 1) * 1000);
	OLED_ShowString_11x18W(11, 11, "EC1:R*");
	OLED_ShowString_11x18W(77, 11, strCount);
}

void ChangeEncoderSmallMultiple(uint8_t cntSmall)
{
	char strCount[6];

	if(cntSmall >= 1)
	{
		EncParam.cntSmallMultiple = 1;
	}
	else
	{
		EncParam.cntSmallMultiple = 0;
	}
	OLED_Clear(0);
	sprintf(strCount, "%d", (EncParam.cntSmallMultiple + 1) * 50);
	OLED_ShowString_11x18W(11, 11, "EC2:R*");
	OLED_ShowString_11x18W(77, 11, strCount);
}

// Encoder External Interrupt (Rising Edge): EC22=PD0, EC21=PD1, EC12=PD2, EC11=PD3
void HAL_GPIO_EXTI_Rising_Callback(uint16_t pinEc)
{
	ExCbkParam.isRisingEdge = true;
	ExCbkParam.tickRising = HAL_GetTick();

    switch(pinEc)
    {
    	case EC22_EXTI0_Pin:
    		if(!HAL_GPIO_ReadPin(EC21_EXTI1_GPIO_Port, EC21_EXTI1_Pin))
    		{
    			// Right rotation
    			EncParam.isCountUp = true;
    			EncParam.cntEC2++;
    		}
    		break;
    	case EC21_EXTI1_Pin:
    		if(!HAL_GPIO_ReadPin(EC22_EXTI0_GPIO_Port, EC22_EXTI0_Pin))
    		{
    			// Left rotation
    			EncParam.isCountUp = false;
    			EncParam.cntEC2--;
    		}
    		break;
    	case EC12_EXTI2_Pin:
    		if(!HAL_GPIO_ReadPin(EC11_EXTI3_GPIO_Port, EC11_EXTI3_Pin))
    		{
    			// Right rotation
    			EncParam.isCountUp = true;
    			EncParam.cntEC1++;
    		}
    		break;
    	case EC11_EXTI3_Pin:
    		if(!HAL_GPIO_ReadPin(EC12_EXTI2_GPIO_Port, EC12_EXTI2_Pin))
    		{
    			// Left rotation
    			EncParam.isCountUp = false;
    			EncParam.cntEC1--;
    		}
    		break;
    }
}

// Encoder External Interrupt (Falling Edge): EC22=PD0, EC21=PD1, EC12=PD2, EC11=PD3
void HAL_GPIO_EXTI_Falling_Callback(uint16_t pinEc)
{
	ExCbkParam.isFallingEdge = true;
	ExCbkParam.tickFalling = HAL_GetTick();

    switch(pinEc)
    {
    	case EC22_EXTI0_Pin:
    		if(HAL_GPIO_ReadPin(EC21_EXTI1_GPIO_Port, EC21_EXTI1_Pin))
    		{
    			// Right rotation
    			EncParam.isCountUp = true;
    			EncParam.cntEC2++;
    		}
    		break;
    	case EC21_EXTI1_Pin:
    		if(HAL_GPIO_ReadPin(EC22_EXTI0_GPIO_Port, EC22_EXTI0_Pin))
    		{
    			// Left rotation
    			EncParam.isCountUp = false;
    			EncParam.cntEC2--;
    		}
    		break;
    	case EC12_EXTI2_Pin:
    		if(HAL_GPIO_ReadPin(EC11_EXTI3_GPIO_Port, EC11_EXTI3_Pin))
    		{
    			// Right rotation
    			EncParam.isCountUp = true;
    			EncParam.cntEC1++;
    		}
    		break;
    	case EC11_EXTI3_Pin:
    		if(HAL_GPIO_ReadPin(EC12_EXTI2_GPIO_Port, EC12_EXTI2_Pin))
    		{
    			// Left rotation
    			EncParam.isCountUp = false;
    			EncParam.cntEC1--;
    		}
    		break;
    }
}

EncoderParam_t ReadEncoderParam(void)
{
	return EncParam;
}

void WriteEncoderParam(EncoderParam_t param)
{
	memcpy(&EncParam, &param, sizeof(EncoderParam_t));
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
	EncParam.cntBigMultiple = 0;
	EncParam.cntSmallMultiple = 0;
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
