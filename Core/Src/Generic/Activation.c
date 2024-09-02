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

void WriteRelay(RelayPin rly, GPIO_PinState sta)
{
	if(rly == Relay1)
	{
		HAL_GPIO_WritePin(RLY_1_GPIO_Port, RLY_1_Pin, sta);
	}
	else if(rly == Relay2)
	{
		HAL_GPIO_WritePin(RLY_2_GPIO_Port, RLY_2_Pin, sta);
	}
	else if(rly == Relay3)
	{
		HAL_GPIO_WritePin(RLY_3_GPIO_Port, RLY_3_Pin, sta);
	}
	else if(rly == Relay4)
	{
		HAL_GPIO_WritePin(RLY_4_GPIO_Port, RLY_4_Pin, sta);
	}
	else if(rly == Relay5)
	{
		HAL_GPIO_WritePin(RLY_5_GPIO_Port, RLY_5_Pin, sta);
	}
	else if(rly == Relay6)
	{
		HAL_GPIO_WritePin(RLY_6_GPIO_Port, RLY_6_Pin, sta);
	}
	else if(rly == Relay7)
	{
		HAL_GPIO_WritePin(RLY_7_GPIO_Port, RLY_7_Pin, sta);
	}
	else if(rly == Relay8)
	{
		HAL_GPIO_WritePin(RLY_8_GPIO_Port, RLY_8_Pin, sta);
	}
	else if(rly == Relay9)
	{
		HAL_GPIO_WritePin(RLY_9_GPIO_Port, RLY_9_Pin, sta);
	}
	else if(rly == RelayOpen)
	{
		HAL_GPIO_WritePin(RLY_Open_GPIO_Port, RLY_Open_Pin, sta);
	}
	else if(rly == RelayShort)
	{
		HAL_GPIO_WritePin(RLY_Short_GPIO_Port, RLY_Short_Pin, sta);
	}
}

void LedAllOff(void)
{
	WriteLED(LED1, 0);
	WriteLED(LED2, 0);
	WriteLED(LED3, 0);
	WriteLED(LED4, 0);
	WriteLED(LED5, 0);
}

void RelayAllOff(void)
{
	WriteRelay(Relay1, 0);
	WriteRelay(Relay2, 0);
	WriteRelay(Relay3, 0);
	WriteRelay(Relay4, 0);
	WriteRelay(Relay5, 0);
	WriteRelay(Relay6, 0);
	WriteRelay(Relay7, 0);
	WriteRelay(Relay8, 0);
	WriteRelay(Relay9, 0);
	WriteRelay(RelayOpen, 0);
	WriteRelay(RelayShort, 0);
}
