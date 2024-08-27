/*
 * TestButton.c
 *
 *  Created on: Aug 27, 2024
 *      Author: Sam
 */

#include "main.h"
#include "TestFunc/TestFunc.h"
#include "Display/OLEDCtrl.h"

static void ShowCurrentParameter(void)
{
	SEGGER_RTT_printf(0, "---> Section of Button Test Function <---\r\n");
	SEGGER_RTT_printf(0, "-> Click Specific Key to Activate Function\r\n");
	SEGGER_RTT_printf(0, "-> 1) PS1 Button Test\r\n");
	SEGGER_RTT_printf(0, "-> 2) PS2 Button Test\r\n");
	SEGGER_RTT_printf(0, "-> 3) PS3 Button Test\r\n");
	SEGGER_RTT_printf(0, "-> 4) PS4 Button Test\r\n");
	SEGGER_RTT_printf(0, "-> 5) PS5 Button Test\r\n");
	SEGGER_RTT_printf(0, "-> 6) PS6 Button Test\r\n");
	SEGGER_RTT_printf(0, "-> 7) PS7 Button Test\r\n");
	SEGGER_RTT_printf(0, "-> 8) PS8 Button Test\r\n");
	SEGGER_RTT_printf(0, "-> 0) Quit\r\n");
}

static void LedAllOff(void)
{
	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_RESET);
}

void TestButtonTask(void)
{
	int keyOfPrgs;

	char strBtn[] = {"Button_X"};

	ShowCurrentParameter();
	OLED_ShowString_11x18("Button Test");

	do
	{
		keyOfPrgs = SEGGER_RTT_GetKey();
		if(keyOfPrgs == '0')
		{
			LedAllOff();
			break;
		}

		if(HAL_GPIO_ReadPin(PS1_GPIO_Port, PS1_Pin) == GPIO_PIN_RESET)
		{
			LedAllOff();
			HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
			strBtn[7] = '1';
			OLED_ShowString_11x18(strBtn);
			SEGGER_RTT_printf(0, "-> Press PS1 Button\r\n");
		    HAL_Delay(500);
		}
		if(HAL_GPIO_ReadPin(PS2_GPIO_Port, PS2_Pin) == GPIO_PIN_RESET)
		{
			LedAllOff();
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
			strBtn[7] = '2';
			OLED_ShowString_11x18(strBtn);
			SEGGER_RTT_printf(0, "-> Press PS2 Button\r\n");
		    HAL_Delay(500);
		}
		if(HAL_GPIO_ReadPin(PS3_GPIO_Port, PS3_Pin) == GPIO_PIN_RESET)
		{
			LedAllOff();
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
			strBtn[7] = '3';
			OLED_ShowString_11x18(strBtn);
			SEGGER_RTT_printf(0, "-> Press PS3 Button\r\n");
		    HAL_Delay(500);
		}
		if(HAL_GPIO_ReadPin(PS4_GPIO_Port, PS4_Pin) == GPIO_PIN_RESET)
		{
			LedAllOff();
			HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_SET);
			strBtn[7] = '4';
			OLED_ShowString_11x18(strBtn);
			SEGGER_RTT_printf(0, "-> Press PS4 Button\r\n");
		    HAL_Delay(500);
		}
		if(HAL_GPIO_ReadPin(PS5_GPIO_Port, PS5_Pin) == GPIO_PIN_RESET)
		{
			LedAllOff();
			HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_SET);
			strBtn[7] = '5';
			OLED_ShowString_11x18(strBtn);
			SEGGER_RTT_printf(0, "-> Press PS5 Button\r\n");
		    HAL_Delay(500);
		}
		if(HAL_GPIO_ReadPin(PS6_GPIO_Port, PS6_Pin) == GPIO_PIN_RESET)
		{
			LedAllOff();
			HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
			strBtn[7] = '6';
			OLED_ShowString_11x18(strBtn);
			SEGGER_RTT_printf(0, "-> Press PS6 Button\r\n");
		    HAL_Delay(500);
		}
		if(HAL_GPIO_ReadPin(PS7_GPIO_Port, PS7_Pin) == GPIO_PIN_RESET)
		{
			LedAllOff();
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_SET);
			strBtn[7] = '7';
			OLED_ShowString_11x18(strBtn);
			SEGGER_RTT_printf(0, "-> Press PS7 Button\r\n");
		    HAL_Delay(500);
		}
		if(HAL_GPIO_ReadPin(PS8_GPIO_Port, PS8_Pin) == GPIO_PIN_RESET)
		{
			LedAllOff();
			HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_SET);
			strBtn[7] = '8';
			OLED_ShowString_11x18(strBtn);
			SEGGER_RTT_printf(0, "-> Press PS8 Button\r\n");
		    HAL_Delay(500);
		}
	}
	while(1);
}
