/*
 * Encoder.c
 *
 *  Created on: Aug 29, 2024
 *      Author: Sam
 */

#include "RTT/SEGGER_RTT.h"
#include "Generic/Encoder.h"
#include "Generic/Activation.h"
#include "Display/OLEDCtrl.h"

static EncoderParam_t EncParam =
{
	.SignalRisingEXTI = 0,
	.SignalFallingEXTI = 0,
	.PinEXTI = 0,
	.cntEC2 = 0,
	.cntEC1 = 0,
	.cntLED = 0,
	.isCountUp = 0,
	.isShowOLED = 0,
};

static void EncoderSignalRisingHandler(uint16_t pinEc)
{
    switch(pinEc)
    {
    	case EC22_EXTI0_Pin:
			if(EncParam.isCountUp)
			{
				if(EncParam.cntLED == 1)
				{
					WriteLED(LED3, 1);
				}
				else if(EncParam.cntLED == 2)
				{
					WriteLED(LED4, 1);
				}
				else if(EncParam.cntLED == 3)
				{
					WriteLED(LED5, 1);
				}
				else if(EncParam.cntLED == 4)
				{
					WriteLED(LED2, 1);
				}
				else
				{
					WriteLED(LED1, 1);
				}
			}
    		break;
    	case EC21_EXTI1_Pin:
			if(!EncParam.isCountUp)
			{
				if(EncParam.cntLED == 0)
				{
					WriteLED(LED3, 0);
				}
				else if(EncParam.cntLED == 1)
				{
					WriteLED(LED4, 0);
				}
				else if(EncParam.cntLED == 2)
				{
					WriteLED(LED5, 0);
				}
				else if(EncParam.cntLED == 3)
				{
					WriteLED(LED2, 0);
				}
				else
				{
					WriteLED(LED1, 0);
				}
			}
    		break;
    	case EC12_EXTI2_Pin:
			if(EncParam.isCountUp)
			{
				if(EncParam.cntLED == 1)
				{
					WriteLED(LED3, 1);
				}
				else if(EncParam.cntLED == 2)
				{
					WriteLED(LED4, 1);
				}
				else if(EncParam.cntLED == 3)
				{
					WriteLED(LED5, 1);
				}
				else if(EncParam.cntLED == 4)
				{
					WriteLED(LED2, 1);
				}
				else
				{
					WriteLED(LED1, 1);
				}
			}
    		break;
    	case EC11_EXTI3_Pin:
			if(!EncParam.isCountUp)
			{
				if(EncParam.cntLED == 0)
				{
					WriteLED(LED3, 0);
				}
				else if(EncParam.cntLED == 1)
				{
					WriteLED(LED4, 0);
				}
				else if(EncParam.cntLED == 2)
				{
					WriteLED(LED5, 0);
				}
				else if(EncParam.cntLED == 3)
				{
					WriteLED(LED2, 0);
				}
				else
				{
					WriteLED(LED1, 0);
				}
			}
    		break;
    }
}

static void EncoderSignalFallingHandler(uint16_t pinEc)
{
    switch(pinEc)
    {
    	case EC22_EXTI0_Pin:
			if(EncParam.isCountUp)
			{
				if(EncParam.cntLED == 1)
				{
					WriteLED(LED3, 1);
				}
				else if(EncParam.cntLED == 2)
				{
					WriteLED(LED4, 1);
				}
				else if(EncParam.cntLED == 3)
				{
					WriteLED(LED5, 1);
				}
				else if(EncParam.cntLED == 4)
				{
					WriteLED(LED2, 1);
				}
				else
				{
					WriteLED(LED1, 1);
				}
			}
    		break;
    	case EC21_EXTI1_Pin:
			if(!EncParam.isCountUp)
			{
				if(EncParam.cntLED == 0)
				{
					WriteLED(LED3, 0);
				}
				else if(EncParam.cntLED == 1)
				{
					WriteLED(LED4, 0);
				}
				else if(EncParam.cntLED == 2)
				{
					WriteLED(LED5, 0);
				}
				else if(EncParam.cntLED == 3)
				{
					WriteLED(LED2, 0);
				}
				else
				{
					WriteLED(LED1, 0);
				}
			}
    		break;
    	case EC12_EXTI2_Pin:
			if(EncParam.isCountUp)
			{
				if(EncParam.cntLED == 1)
				{
					WriteLED(LED3, 1);
				}
				else if(EncParam.cntLED == 2)
				{
					WriteLED(LED4, 1);
				}
				else if(EncParam.cntLED == 3)
				{
					WriteLED(LED5, 1);
				}
				else if(EncParam.cntLED == 4)
				{
					WriteLED(LED2, 1);
				}
				else
				{
					WriteLED(LED1, 1);
				}
			}
    		break;
    	case EC11_EXTI3_Pin:
			if(!EncParam.isCountUp)
			{
				if(EncParam.cntLED == 0)
				{
					WriteLED(LED3, 0);
				}
				else if(EncParam.cntLED == 1)
				{
					WriteLED(LED4, 0);
				}
				else if(EncParam.cntLED == 2)
				{
					WriteLED(LED5, 0);
				}
				else if(EncParam.cntLED == 3)
				{
					WriteLED(LED2, 0);
				}
				else
				{
					WriteLED(LED1, 0);
				}
			}
    		break;
    }
}

// Encoder External Interrupt Pin: EC22=PD0, EC21=PD1, Ec12=PD2, Ec11=PD3
void HAL_GPIO_EXTI_Rising_Callback(uint16_t pinEc)
{
	EncParam.SignalRisingEXTI = 1;
	EncParam.PinEXTI = pinEc;
    switch(pinEc)
    {
    	case EC22_EXTI0_Pin:
    		if(!HAL_GPIO_ReadPin(EC21_EXTI1_GPIO_Port, EC21_EXTI1_Pin))
    		{
    			// Encoder: "Forward" rotation
    			EncParam.isCountUp = 1;
    			EncParam.cntEC2++;
    			EncParam.cntLED = EncParam.cntEC2 % 5;
    		}
    		break;
    	case EC21_EXTI1_Pin:
    		if(!HAL_GPIO_ReadPin(EC22_EXTI0_GPIO_Port, EC22_EXTI0_Pin))
    		{
    			// Encoder: "Reverse" rotation
    			EncParam.isCountUp = 0;
    			EncParam.cntEC2--;
    			EncParam.cntLED = EncParam.cntEC2 % 5;
    		}
    		break;
    	case EC12_EXTI2_Pin:
    		if(!HAL_GPIO_ReadPin(EC11_EXTI3_GPIO_Port, EC11_EXTI3_Pin))
    		{
    			// Encoder: "Forward" rotation
    			EncParam.isCountUp = 1;
    			EncParam.cntEC1++;
    			EncParam.cntLED = EncParam.cntEC1 % 5;
    		}
    		break;
    	case EC11_EXTI3_Pin:
    		if(!HAL_GPIO_ReadPin(EC12_EXTI2_GPIO_Port, EC12_EXTI2_Pin))
    		{
    			// Encoder: "Reverse" rotation
    			EncParam.isCountUp = 0;
    			EncParam.cntEC1--;
    			EncParam.cntLED = EncParam.cntEC1 % 5;
    		}
    		break;
    }
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t pinEc)
{
	EncParam.SignalFallingEXTI = 1;
	EncParam.PinEXTI = pinEc;
    switch(pinEc)
    {
    	case EC22_EXTI0_Pin:
    		if(HAL_GPIO_ReadPin(EC21_EXTI1_GPIO_Port, EC21_EXTI1_Pin))
    		{
    			// Encoder: "Forward" rotation
    			EncParam.isCountUp = 1;
    			EncParam.cntEC2++;
    			EncParam.cntLED = EncParam.cntEC2 % 5;
    		}
    		break;
    	case EC21_EXTI1_Pin:
    		if(HAL_GPIO_ReadPin(EC22_EXTI0_GPIO_Port, EC22_EXTI0_Pin))
    		{
    			// Encoder: "Reverse" rotation
    			EncParam.isCountUp = 0;
    			EncParam.cntEC2--;
    			EncParam.cntLED = EncParam.cntEC2 % 5;
    		}
    		break;
    	case EC12_EXTI2_Pin:
    		if(HAL_GPIO_ReadPin(EC11_EXTI3_GPIO_Port, EC11_EXTI3_Pin))
    		{
    			// Encoder: "Forward" rotation
    			EncParam.isCountUp = 1;
    			EncParam.cntEC1++;
    			EncParam.cntLED = EncParam.cntEC1 % 5;
    		}
    		break;
    	case EC11_EXTI3_Pin:
    		if(HAL_GPIO_ReadPin(EC12_EXTI2_GPIO_Port, EC12_EXTI2_Pin))
    		{
    			// Encoder: "Reverse" rotation
    			EncParam.isCountUp = 0;
    			EncParam.cntEC1--;
    			EncParam.cntLED = EncParam.cntEC1 % 5;
    		}
    		break;
    }
}

void EncoderTask(void)
{
	static uint16_t cntEC1tmp = 0;
	static uint16_t cntEC2tmp = 0;
	uint16_t cntTmp = 0;

	char strCntEC1[] = {"EC1:   "};
	char strCntEC2[] = {"EC2:   "};

	if(EncParam.SignalRisingEXTI)
	{
		EncParam.SignalRisingEXTI = 0;
		EncoderSignalRisingHandler(EncParam.PinEXTI);
		if(cntEC2tmp != EncParam.cntEC2)
		{
			cntEC2tmp = EncParam.cntEC2;
			OLED_Clear(0);
			cntTmp = cntEC2tmp % 1000;
			strCntEC2[4] = (cntTmp / 100) + '0';
			cntTmp = cntTmp % 100;
			strCntEC2[5] = (cntTmp / 10) + '0';
			strCntEC2[6] = (cntTmp % 10) + '0';
			OLED_ShowString_11x18W(0, 0, strCntEC2);
			OLED_ShowString_7x10W(2, 22, "(EC2 Testing...)");
			SEGGER_RTT_printf(0, "-> Count: %d\r\n", EncParam.cntEC2);
		}
		if(cntEC1tmp != EncParam.cntEC1)
		{
			cntEC1tmp = EncParam.cntEC1;
			OLED_Clear(0);
			cntTmp = cntEC1tmp % 1000;
			strCntEC1[4] = (cntTmp / 100) + '0';
			cntTmp = cntTmp % 100;
			strCntEC1[5] = (cntTmp / 10) + '0';
			strCntEC1[6] = (cntTmp % 10) + '0';
			OLED_ShowString_11x18W(0, 0, strCntEC1);
			OLED_ShowString_7x10W(2, 22, "(EC1 Testing...)");
			SEGGER_RTT_printf(0, "-> Count: %d\r\n", EncParam.cntEC1);
		}
	}
	else if(EncParam.SignalFallingEXTI)
	{
		EncParam.SignalFallingEXTI = 0;
		EncoderSignalFallingHandler(EncParam.PinEXTI);
		if(cntEC2tmp != EncParam.cntEC2)
		{
			cntEC2tmp = EncParam.cntEC2;
			OLED_Clear(0);
			cntTmp = cntEC2tmp % 1000;
			strCntEC2[4] = (cntTmp / 100) + '0';
			cntTmp = cntTmp % 100;
			strCntEC2[5] = (cntTmp / 10) + '0';
			strCntEC2[6] = (cntTmp % 10) + '0';
			OLED_ShowString_11x18W(0, 0, strCntEC2);
			OLED_ShowString_7x10W(2, 22, "(EC2 Testing...)");
			SEGGER_RTT_printf(0, "-> Count: %d\r\n", EncParam.cntEC2);
		}
		if(cntEC1tmp != EncParam.cntEC1)
		{
			cntEC1tmp = EncParam.cntEC1;
			OLED_Clear(0);
			cntTmp = cntEC1tmp % 1000;
			strCntEC1[4] = (cntTmp / 100) + '0';
			cntTmp = cntTmp % 100;
			strCntEC1[5] = (cntTmp / 10) + '0';
			strCntEC1[6] = (cntTmp % 10) + '0';
			OLED_ShowString_11x18W(0, 0, strCntEC1);
			OLED_ShowString_7x10W(2, 22, "(EC1 Testing...)");
			SEGGER_RTT_printf(0, "-> Count: %d\r\n", EncParam.cntEC1);
		}
	}
}
