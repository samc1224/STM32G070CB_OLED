/*
 * Encoder.c
 *
 *  Created on: Aug 29, 2024
 *      Author: Sam
 */

#include "Generic/Encoder.h"
#include "Generic/Activation.h"
#include "TestFunc/TestFunc.h"

static EncoderParam_t EncParam =
{
	.SignalRisingEXTI = 0,
	.SignalFallingEXTI = 0,
	.PinEXTI = 0,
	.Count = 0,
	.cntLED = 0,
	.isCountUpLastTime = 1,
	.isEc22Rise = 0,
	.isEc21Rise = 0,
};

static void EncoderSignalRisingHandler(uint16_t pinEc)
{
    switch(pinEc)
    {
    	case EC22_EXTI0_Pin:
    		if(!HAL_GPIO_ReadPin(EC21_EXTI1_GPIO_Port, EC21_EXTI1_Pin))
    		{
    			// Encoder: "Forward" rotation
    			EncParam.Count++;

    			if(!EncParam.isEc22Rise)
    			{
    				//if(isCountUpLastTime)
    				{
    					EncParam.cntLED++;
						if(EncParam.cntLED > 5)
						{
							EncParam.cntLED = 5;
						}
    				}

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
    				else if(EncParam.cntLED == 5)
    				{
    					WriteLED(LED1, 1);
    				}
    				EncParam.isCountUpLastTime = 1;
    			}
    			EncParam.isEc22Rise = 1;
    		}
    		break;
    	case EC21_EXTI1_Pin:
    		if(!HAL_GPIO_ReadPin(EC22_EXTI0_GPIO_Port, EC22_EXTI0_Pin))
    		{
    			// Encoder: "Reverse" rotation
    			EncParam.Count--;

    			if(!EncParam.isEc21Rise)
    			{
    				if(EncParam.cntLED == 1)
    				{
    					WriteLED(LED3, 0);
    				}
    				else if(EncParam.cntLED == 2)
    				{
    					WriteLED(LED4, 0);
    				}
    				else if(EncParam.cntLED == 3)
    				{
    					WriteLED(LED5, 0);
    				}
    				else if(EncParam.cntLED == 4)
    				{
    					WriteLED(LED2, 0);
    				}
    				else if(EncParam.cntLED == 5)
    				{
    					WriteLED(LED1, 0);
    				}

    				if(EncParam.cntLED > 0)
    				{
						EncParam.cntLED--;
    				}
    				EncParam.isCountUpLastTime = 0;
    			}
    			EncParam.isEc21Rise = 1;
    		}
    		break;
    	case EC12_EXTI2_Pin:
    		break;
    	case EC11_EXTI3_Pin:
    		break;
    }
}

static void EncoderSignalFallingHandler(uint16_t pinEc)
{
    switch(pinEc)
    {
    	case EC22_EXTI0_Pin:
    		if(HAL_GPIO_ReadPin(EC21_EXTI1_GPIO_Port, EC21_EXTI1_Pin))
    		{
    			// Encoder: "Forward" rotation
    			EncParam.Count++;

    			if(EncParam.isEc22Rise)
    			{
    				//if(isCountUpLastTime)
    				{
						EncParam.cntLED++;
						if(EncParam.cntLED > 5)
						{
							EncParam.cntLED = 5;
						}
    				}

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
    				else if(EncParam.cntLED == 5)
    				{
    					WriteLED(LED1, 1);
    				}
    				EncParam.isCountUpLastTime = 1;
    			}
    			EncParam.isEc22Rise = 0;
    		}
    		break;
    	case EC21_EXTI1_Pin:
    		if(HAL_GPIO_ReadPin(EC22_EXTI0_GPIO_Port, EC22_EXTI0_Pin))
    		{
    			// Encoder: "Reverse" rotation
    			EncParam.Count--;

    			if(EncParam.isEc21Rise)
    			{
    				if(EncParam.cntLED == 1)
    				{
    					WriteLED(LED3, 0);
    				}
    				else if(EncParam.cntLED == 2)
    				{
    					WriteLED(LED4, 0);
    				}
    				else if(EncParam.cntLED == 3)
    				{
    					WriteLED(LED5, 0);
    				}
    				else if(EncParam.cntLED == 4)
    				{
    					WriteLED(LED2, 0);
    				}
    				else if(EncParam.cntLED == 5)
    				{
    					WriteLED(LED1, 0);
    				}

    				if(EncParam.cntLED > 0)
    				{
						EncParam.cntLED--;
    				}
    				EncParam.isCountUpLastTime = 0;
    			}
    			EncParam.isEc21Rise = 0;
    		}
    		break;
    	case EC12_EXTI2_Pin:
    		break;
    	case EC11_EXTI3_Pin:
    		break;
    }
}

// Encoder External Interrupt Pin: Ec22=PD0, Ec21=PD1, Ec12=PD2, Ec11=PD3
void HAL_GPIO_EXTI_Rising_Callback(uint16_t pinEc)
{
	EncParam.SignalRisingEXTI = 1;
	EncParam.PinEXTI = pinEc;
	//EncoderTask();
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t pinEc)
{
	EncParam.SignalFallingEXTI = 1;
	EncParam.PinEXTI = pinEc;
	//EncoderTask();
}

void EncoderTask(void)
{
	static uint16_t cntEnc = 0;

	if(EncParam.SignalRisingEXTI)
	{
		EncParam.SignalRisingEXTI = 0;
		EncoderSignalRisingHandler(EncParam.PinEXTI);
		if(cntEnc != EncParam.Count)
		{
			cntEnc = EncParam.Count;
			SEGGER_RTT_printf(0, "-> Count: %d\r\n", EncParam.Count);
		}
	}
	if(EncParam.SignalFallingEXTI)
	{
		EncParam.SignalFallingEXTI = 0;
		EncoderSignalFallingHandler(EncParam.PinEXTI);
		if(cntEnc != EncParam.Count)
		{
			cntEnc = EncParam.Count;
			SEGGER_RTT_printf(0, "-> Count: %d\r\n", EncParam.Count);
		}
	}
}
