/*
 * Encoder.c
 *
 *  Created on: Aug 29, 2024
 *      Author: Sam
 */

#include "Generic/Encoder.h"
#include "Generic/Activation.h"
#include "Display/OLEDCtrl.h"

#include "RTT/SEGGER_RTT.h"

static EncoderParam_t EncParam =
{
	.SignalRisingEXTI = 0,
	.SignalFallingEXTI = 0,
	.cntRelay = 0,
	.cntEC1 = 0,
	.cntEC2 = 0,
	.isCountUp = 0,
};

static void ConvertRelayState(uint16_t sta)
{
	if(sta != 0x00)
		WriteRelay(RelayOpen, 1);
	else
		WriteRelay(RelayOpen, 0);

	if(sta & 0x100)
		WriteRelay(Relay1, 1);
	else
		WriteRelay(Relay1, 0);
	if(sta & 0x80)
		WriteRelay(Relay2, 1);
	else
		WriteRelay(Relay2, 0);
	if(sta & 0x40)
		WriteRelay(Relay3, 1);
	else
		WriteRelay(Relay3, 0);
	if(sta & 0x20)
		WriteRelay(Relay4, 1);
	else
		WriteRelay(Relay4, 0);
	if(sta & 0x10)
		WriteRelay(Relay5, 1);
	else
		WriteRelay(Relay5, 0);
	if(sta & 0x08)
		WriteRelay(Relay6, 1);
	else
		WriteRelay(Relay6, 0);
	if(sta & 0x04)
		WriteRelay(Relay7, 1);
	else
		WriteRelay(Relay7, 0);
	if(sta & 0x02)
		WriteRelay(Relay8, 1);
	else
		WriteRelay(Relay8, 0);
	if(sta & 0x01)
		WriteRelay(Relay9, 1);
	else
		WriteRelay(Relay9, 0);
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

/*
static void CountToString(char *arr, uint16_t cnt)
{
	arr[0]= (cnt / 10000) + '0';
	cnt = cnt % 10000;
	arr[1]= (cnt / 1000) + '0';
	cnt = cnt % 1000;
	arr[2] = (cnt / 100) + '0';
	cnt = cnt % 100;
	arr[3] = (cnt / 10) + '0';
	arr[4] = (cnt % 10) + '0';
	arr[5]= 0;
}

static void LedRunLeftRight(uint16_t cntNow, uint16_t cntOld)
{
	static uint8_t cntLED = 0;
	uint8_t arrLED[5] = { 0x10, 0x08, 0x04, 0x02, 0x01 };

	if(cntNow > cntOld)
	{
		cntLED++;
		if(cntLED > 5)
		{
			cntLED = 5;
		}
		ConvertLedState(arrLED[cntLED]);
		ConvertRelayState(arrLED[cntLED]);
	}
	else
	{
		if(cntLED > 0 && cntLED <= 5)
		{
			cntLED--;
		}
		else
		{
			cntLED = 0;
		}
		ConvertLedState(arrLED[cntLED]);
		ConvertRelayState(arrLED[cntLED]);
	}
}
*/

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

static void RelayBigChange(uint16_t cntNow, uint16_t cntOld)
{
	static uint8_t cntState = 0;
	uint16_t arrState[10] = { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0x1FF };

	for(int i=9; i>=0; i--)
	{
		if(EncParam.cntRelay >= arrState[i])
		{
			cntState = i;
			break;
		}
	}

	if(cntNow > cntOld)
	{
		cntState++;
		if(cntState > 9)
		{
			cntState = 9;
		}
		ConvertLedState(arrState[cntState]);
		ConvertRelayState(arrState[cntState]);
	}
	else
	{
		if(cntState > 0 && cntState <= 9)
		{
			cntState--;
		}
		else
		{
			cntState = 0;
		}
		ConvertLedState(arrState[cntState]);
		ConvertRelayState(arrState[cntState]);
	}

	EncParam.cntRelay = arrState[cntState];
	ShowEncoderCount();
}

static void RelaySmallChange(uint16_t cntNow, uint16_t cntOld)
{
	if(cntNow > cntOld)
	{
		EncParam.cntRelay++;
		if(EncParam.cntRelay > 0x1FF)
		{
			EncParam.cntRelay = 0x1FF;
		}
		ConvertLedState(EncParam.cntRelay);
		ConvertRelayState(EncParam.cntRelay);
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
		ConvertLedState(EncParam.cntRelay);
		ConvertRelayState(EncParam.cntRelay);
	}

	ShowEncoderCount();
}

// Encoder External Interrupt Pin: EC22=PD0, EC21=PD1, Ec12=PD2, Ec11=PD3
void HAL_GPIO_EXTI_Rising_Callback(uint16_t pinEc)
{
	EncParam.SignalRisingEXTI = 1;
    switch(pinEc)
    {
    	case EC22_EXTI0_Pin:
    		if(!HAL_GPIO_ReadPin(EC21_EXTI1_GPIO_Port, EC21_EXTI1_Pin))
    		{
    			// Encoder: "Forward" rotation
    			EncParam.isCountUp = 1;
    			EncParam.cntEC2++;
    			//if(EncParam.cntEC2 > 1000)
    			//	EncParam.cntEC2 = 1000;
    		}
    		break;
    	case EC21_EXTI1_Pin:
    		if(!HAL_GPIO_ReadPin(EC22_EXTI0_GPIO_Port, EC22_EXTI0_Pin))
    		{
    			// Encoder: "Reverse" rotation
    			EncParam.isCountUp = 0;
    			EncParam.cntEC2--;
    			//if(EncParam.cntEC2 > 1000)
    			//	EncParam.cntEC2 = 0;
    		}
    		break;
    	case EC12_EXTI2_Pin:
    		if(!HAL_GPIO_ReadPin(EC11_EXTI3_GPIO_Port, EC11_EXTI3_Pin))
    		{
    			// Encoder: "Forward" rotation
    			EncParam.isCountUp = 1;
    			EncParam.cntEC1++;
    			//if(EncParam.cntEC1 > 1000)
    			//	EncParam.cntEC1 = 1000;
    		}
    		break;
    	case EC11_EXTI3_Pin:
    		if(!HAL_GPIO_ReadPin(EC12_EXTI2_GPIO_Port, EC12_EXTI2_Pin))
    		{
    			// Encoder: "Reverse" rotation
    			EncParam.isCountUp = 0;
    			EncParam.cntEC1--;
    			//if(EncParam.cntEC1 > 1000)
    			//	EncParam.cntEC1 = 0;
    		}
    		break;
    }
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t pinEc)
{
	EncParam.SignalFallingEXTI = 1;
    switch(pinEc)
    {
    	case EC22_EXTI0_Pin:
    		if(HAL_GPIO_ReadPin(EC21_EXTI1_GPIO_Port, EC21_EXTI1_Pin))
    		{
    			// Encoder: "Forward" rotation
    			EncParam.isCountUp = 1;
    			EncParam.cntEC2++;
    			//if(EncParam.cntEC2 > 1000)
    			//	EncParam.cntEC2 = 1000;
    		}
    		break;
    	case EC21_EXTI1_Pin:
    		if(HAL_GPIO_ReadPin(EC22_EXTI0_GPIO_Port, EC22_EXTI0_Pin))
    		{
    			// Encoder: "Reverse" rotation
    			EncParam.isCountUp = 0;
    			EncParam.cntEC2--;
    			//if(EncParam.cntEC2 > 1000)
    			//	EncParam.cntEC2 = 0;
    		}
    		break;
    	case EC12_EXTI2_Pin:
    		if(HAL_GPIO_ReadPin(EC11_EXTI3_GPIO_Port, EC11_EXTI3_Pin))
    		{
    			// Encoder: "Forward" rotation
    			EncParam.isCountUp = 1;
    			EncParam.cntEC1++;
    			//if(EncParam.cntEC1 > 1000)
    			//	EncParam.cntEC1 = 1000;
    		}
    		break;
    	case EC11_EXTI3_Pin:
    		if(HAL_GPIO_ReadPin(EC12_EXTI2_GPIO_Port, EC12_EXTI2_Pin))
    		{
    			// Encoder: "Reverse" rotation
    			EncParam.isCountUp = 0;
    			EncParam.cntEC1--;
    			//if(EncParam.cntEC1 > 1000)
    			//	EncParam.cntEC1 = 0;
    		}
    		break;
    }
}

void EncoderTask(void)
{
	static uint16_t cntEC1tmp = 0;
	static uint16_t cntEC2tmp = 0;

	if(EncParam.SignalRisingEXTI)
	{
		EncParam.SignalRisingEXTI = 0;
		if(cntEC2tmp != EncParam.cntEC2)
		{
			RelayBigChange(EncParam.cntEC2, cntEC2tmp);
			cntEC2tmp = EncParam.cntEC2;
			SEGGER_RTT_printf(0, "-> Count: %d\r\n", EncParam.cntEC2);
		}
		if(cntEC1tmp != EncParam.cntEC1)
		{
			RelaySmallChange(EncParam.cntEC1, cntEC1tmp);
			cntEC1tmp = EncParam.cntEC1;
			SEGGER_RTT_printf(0, "-> Count: %d\r\n", EncParam.cntEC1);
		}
	}
	else if(EncParam.SignalFallingEXTI)
	{
		EncParam.SignalFallingEXTI = 0;
		if(cntEC2tmp != EncParam.cntEC2)
		{
			RelayBigChange(EncParam.cntEC2, cntEC2tmp);
			cntEC2tmp = EncParam.cntEC2;
			SEGGER_RTT_printf(0, "-> Count: %d\r\n", EncParam.cntEC2);
		}
		if(cntEC1tmp != EncParam.cntEC1)
		{
			RelaySmallChange(EncParam.cntEC1, cntEC1tmp);
			cntEC1tmp = EncParam.cntEC1;
			SEGGER_RTT_printf(0, "-> Count: %d\r\n", EncParam.cntEC1);
		}
	}
}
