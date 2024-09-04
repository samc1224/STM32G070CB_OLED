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

static EncoderParam_t EncParam =
{
	.EXTI_RisingEdge = 0,
	.EXTI_FallingEdge = 0,
	.isShowCount = 0,
	.isCountUp = 0,
	.cntEC1 = 0,
	.cntEC2 = 0,
	.cntIndex = 0,
	.cntRelay = 0,
};

static void ConvertRelayState(uint16_t sta)
{
	if(sta != 0x00)
	{
		WriteRelay(RelayOpen, 1);
	}
	else
	{
		WriteRelay(RelayOpen, 0);
	}

	for(int i = 0; i < 9; i++)
	{
		if((sta >> i) & 0x001)
		{
			WriteRelay(Relay9 - i, 1);
		}
		else
		{
			WriteRelay(Relay9 - i, 0);
		}
	}
}

static void ConvertLedState(uint8_t sta)
{
	if(sta & 0x10)
		WriteLED(LED3, 1);
	else
		WriteLED(LED3, 0);
	if(sta & 0x08)
		WriteLED(LED4, 1);
	else
		WriteLED(LED4, 0);
	if(sta & 0x04)
		WriteLED(LED5, 1);
	else
		WriteLED(LED5, 0);
	if(sta & 0x02)
		WriteLED(LED2, 1);
	else
		WriteLED(LED2, 0);
	if(sta & 0x01)
		WriteLED(LED1, 1);
	else
		WriteLED(LED1, 0);
}

static void ShowEncoderCount(void)
{
	char strCount[10];

	OLED_Clear(0);
	sprintf(strCount, "%d", EncParam.cntRelay);
	OLED_ShowString_11x18W(0, 0, "Relay:");
	OLED_ShowString_11x18W(68, 0, strCount);
	sprintf(strCount, "%d", EncParam.cntEC1);
	OLED_ShowString_7x10W(2, 22, "E1:");
	OLED_ShowString_7x10W(23, 22, strCount);
	sprintf(strCount, "%d", EncParam.cntEC2);
	OLED_ShowString_7x10W(58, 22, ",E2:");
	OLED_ShowString_7x10W(86, 22, strCount);
}

static void RelayBigChange(bool isCntUp)
{
	uint16_t arrRelayState[10] = { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0x1FF };

	for(int i = 9; i >= 0; i--)
	{
		if(EncParam.cntRelay >= arrRelayState[i])
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
	ConvertLedState(arrRelayState[EncParam.cntIndex]);
	ConvertRelayState(arrRelayState[EncParam.cntIndex]);
	EncParam.cntRelay = arrRelayState[EncParam.cntIndex];
	if(EncParam.isShowCount)
	{
		ShowEncoderCount();
	}
}

static void RelaySmallChange(bool isCntUp)
{
	if(isCntUp)
	{
		EncParam.cntRelay++;
		if(EncParam.cntRelay > 0x1FF)
		{
			EncParam.cntRelay = 0x1FF;
		}
	}
	else
	{
		if(EncParam.cntRelay > 0 && EncParam.cntRelay <= 0x1FF)
		{
			EncParam.cntRelay--;
		}
		else
		{
			EncParam.cntRelay = 0;
		}
	}
	ConvertLedState(EncParam.cntRelay);
	ConvertRelayState(EncParam.cntRelay);
	if(EncParam.isShowCount)
	{
		ShowEncoderCount();
	}
}

// Encoder External Interrupt (Rising Edge): EC22=PD0, EC21=PD1, Ec12=PD2, Ec11=PD3
void HAL_GPIO_EXTI_Rising_Callback(uint16_t pinEc)
{
	EncParam.EXTI_RisingEdge = true;
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
	EncParam.EXTI_FallingEdge = true;
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

EncoderParam_t ReadEncoderParam(void)
{
	return EncParam;
}

void WriteEncoderParam(EncoderParam_t param)
{
	EncParam.cntEC1 = param.cntEC1;
	EncParam.cntEC2 = param.cntEC2;
	EncParam.cntIndex = param.cntIndex;
	EncParam.cntRelay = param.cntRelay;
}

void SetEncoderShowCount(bool setShow)
{
	EncParam.isShowCount = setShow;
}

void EncoderInit(void)
{
	EncParam.EXTI_RisingEdge = 0;
	EncParam.EXTI_FallingEdge = 0;
	EncParam.isCountUp = 0;
	EncParam.cntEC1 = 0;
	EncParam.cntEC2 = 0;
	EncParam.cntIndex = 0;
	EncParam.cntRelay = 0;
}

//In this task, 2 Encoders are used to control the Relay and display the count value on OLED.
void EncoderTask(void)
{
	static uint16_t cntEC1tmp = 0;
	static uint16_t cntEC2tmp = 0;

	if(EncParam.EXTI_RisingEdge)
	{
		EncParam.EXTI_RisingEdge = false;
		if(cntEC2tmp != EncParam.cntEC2)
		{
			RelayBigChange(EncParam.isCountUp);
			cntEC2tmp = EncParam.cntEC2;
			SEGGER_RTT_printf(0, "-> (EC2: %d, Relay: %d)\r\n", EncParam.cntEC2, EncParam.cntRelay);
		}
		if(cntEC1tmp != EncParam.cntEC1)
		{
			RelaySmallChange(EncParam.isCountUp);
			cntEC1tmp = EncParam.cntEC1;
			SEGGER_RTT_printf(0, "-> (EC1: %d, Relay: %d)\r\n", EncParam.cntEC1, EncParam.cntRelay);
		}
	}
	else if(EncParam.EXTI_FallingEdge)
	{
		EncParam.EXTI_FallingEdge = false;
		if(cntEC2tmp != EncParam.cntEC2)
		{
			RelayBigChange(EncParam.isCountUp);
			cntEC2tmp = EncParam.cntEC2;
			SEGGER_RTT_printf(0, "-> (EC2: %d, Relay: %d)\r\n", EncParam.cntEC2, EncParam.cntRelay);
		}
		if(cntEC1tmp != EncParam.cntEC1)
		{
			RelaySmallChange(EncParam.isCountUp);
			cntEC1tmp = EncParam.cntEC1;
			SEGGER_RTT_printf(0, "-> (EC1: %d, Relay: %d)\r\n", EncParam.cntEC1, EncParam.cntRelay);
		}
	}
}
