/*
 * Activation.c
 *
 *  Created on: Aug 28, 2024
 *      Author: Sam
 */

#include "Generic/Activation.h"

GPIO_PinState ReadButton(ButtonPin btn)
{
	if(btn == Button1)
	{
		return HAL_GPIO_ReadPin(PS1_GPIO_Port, PS1_Pin);
	}
	else if(btn == Button2)
	{
		return HAL_GPIO_ReadPin(PS2_GPIO_Port, PS2_Pin);
	}
	else if(btn == Button3)
	{
		return HAL_GPIO_ReadPin(PS3_GPIO_Port, PS3_Pin);
	}
	else if(btn == Button4)
	{
		return HAL_GPIO_ReadPin(PS4_GPIO_Port, PS4_Pin);
	}
	else if(btn == Button5)
	{
		return HAL_GPIO_ReadPin(PS5_GPIO_Port, PS5_Pin);
	}
	else if(btn == Button6)
	{
		return HAL_GPIO_ReadPin(PS6_GPIO_Port, PS6_Pin);
	}
	else if(btn == Button7)
	{
		return HAL_GPIO_ReadPin(PS7_GPIO_Port, PS7_Pin);
	}
	else if(btn == Button8)
	{
		return HAL_GPIO_ReadPin(PS8_GPIO_Port, PS8_Pin);
	}
	else
	{
		return GPIO_PIN_RESET;
	}
}

void WriteLED(LEDPin led, GPIO_PinState sta)
{
	if(led == LED1)
	{
		HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, sta);
	}
	else if(led == LED2)
	{
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, sta);
	}
	else if(led == LED3)
	{
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, sta);
	}
	else if(led == LED4)
	{
		HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, sta);
	}
	else if(led == LED5)
	{
		HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, sta);
	}
}
