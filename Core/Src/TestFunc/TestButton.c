/*
 * TestButton.c
 *
 *  Created on: Aug 27, 2024
 *      Author: Sam
 */

#include "main.h"
#include "TestFunc/TestFunc.h"
#include "Display/OLEDCtrl.h"
#include "Generic/Activation.h"
#include "Comm/Postman.h"

static void ShowCurrentParameter(void)
{
	SEGGER_RTT_printf(0, "***********************************************\r\n");
	SEGGER_RTT_printf(0, "* Current Operation: Button Test              *\r\n");
	SEGGER_RTT_printf(0, "* Click Specific Key to Activate Function.    *\r\n");
	SEGGER_RTT_printf(0, "***********************************************\r\n");
	SEGGER_RTT_printf(0, "-> 1) PS1 Button Test\r\n");
	SEGGER_RTT_printf(0, "-> 2) PS2 Button Test\r\n");
	SEGGER_RTT_printf(0, "-> 3) PS3 Button Test\r\n");
	SEGGER_RTT_printf(0, "-> 4) PS4 Button Test\r\n");
	SEGGER_RTT_printf(0, "-> 5) PS5 Button Test\r\n");
	SEGGER_RTT_printf(0, "-> 6) PS6 Button Test\r\n");
	SEGGER_RTT_printf(0, "-> 7) PS7 Button Test\r\n");
	SEGGER_RTT_printf(0, "-> 8) PS8 Button Test\r\n");
	SEGGER_RTT_printf(0, "-> 0) Quit\r\n\r\n");
}

void TestButtonTask(void)
{
	int keyOfPrgs;
	uint8_t virtualBtn;

	char strBtn[] = {"PSx_Button"};

	OLED_Clear(0);
	OLED_ShowString_11x18W(6, 11, "Button Test");
    HAL_Delay(300);
	ShowCurrentParameter();

	do
	{
		PostmanTask();
		virtualBtn = GetVirtualButton();
		keyOfPrgs = SEGGER_RTT_GetKey();
		if(keyOfPrgs == '0')
		{
			LedAllOff();
			break;
		}

		if(!ReadButton(Button1) || virtualBtn == Button1)
		{
			LedAllOff();
			WriteLED(LED3, 1);
			strBtn[2] = '1';
			OLED_Clear(0);
			OLED_ShowString_11x18W(11, 0, strBtn);
			OLED_ShowString_7x10W(33, 22, "(LED-3 On)");
			SEGGER_RTT_printf(0, "-> (Press PS1 Button)\r\n");
		    HAL_Delay(300);
		}
		if(!ReadButton(Button2) || virtualBtn == Button2)
		{
			LedAllOff();
			WriteLED(LED4, 1);
			strBtn[2] = '2';
			OLED_Clear(0);
			OLED_ShowString_11x18W(11, 0, strBtn);
			OLED_ShowString_7x10W(33, 22, "(LED-4 On)");
			SEGGER_RTT_printf(0, "-> (Press PS2 Button)\r\n");
		    HAL_Delay(300);
		}
		if(!ReadButton(Button3) || virtualBtn == Button3)
		{
			LedAllOff();
			WriteLED(LED5, 1);
			strBtn[2] = '3';
			OLED_Clear(0);
			OLED_ShowString_11x18W(11, 0, strBtn);
			OLED_ShowString_7x10W(33, 22, "(LED-5 On)");
			SEGGER_RTT_printf(0, "-> (Press PS3 Button)\r\n");
		    HAL_Delay(300);
		}
		if(!ReadButton(Button4) || virtualBtn == Button4)
		{
			LedAllOff();
			WriteLED(LED2, 1);
			strBtn[2] = '4';
			OLED_Clear(0);
			OLED_ShowString_11x18W(11, 0, strBtn);
			OLED_ShowString_7x10W(33, 22, "(LED-2 On)");
			SEGGER_RTT_printf(0, "-> (Press PS4 Button)\r\n");
		    HAL_Delay(300);
		}
		if(!ReadButton(Button5) || virtualBtn == Button5)
		{
			LedAllOff();
			WriteLED(LED1, 1);
			strBtn[2] = '5';
			OLED_Clear(0);
			OLED_ShowString_11x18W(11, 0, strBtn);
			OLED_ShowString_7x10W(33, 22, "(LED-1 On)");
			SEGGER_RTT_printf(0, "-> (Press PS5 Button)\r\n");
		    HAL_Delay(300);
		}
		if(!ReadButton(Button6) || virtualBtn == Button6)
		{
			LedAllOff();
			WriteLED(LED3, 1);
			WriteLED(LED4, 1);
			WriteLED(LED5, 1);
			strBtn[2] = '6';
			OLED_Clear(0);
			OLED_ShowString_11x18W(11, 0, strBtn);
			OLED_ShowString_7x10W(18, 22, "(LED 3~5 On)");
			SEGGER_RTT_printf(0, "-> (Press PS6 Button)\r\n");
		    HAL_Delay(300);
		}
		if(!ReadButton(Button7) || virtualBtn == Button7)
		{
			LedAllOff();
			WriteLED(LED2, 1);
			WriteLED(LED1, 1);
			strBtn[2] = '7';
			OLED_Clear(0);
			OLED_ShowString_11x18W(11, 0, strBtn);
			OLED_ShowString_7x10W(18, 22, "(LED 2~1 On)");
			SEGGER_RTT_printf(0, "-> (Press PS7 Button)\r\n");
		    HAL_Delay(300);
		}
		if(!ReadButton(Button8) || virtualBtn == Button8)
		{
			LedAllOff();
			OLED_Clear(0);
			OLED_ShowString_11x18W(22, 11, "(Return)");
			SEGGER_RTT_printf(0, "-> (Press PS8 Button)\r\n\r\n");
		    HAL_Delay(500);
			break;
		}
	}
	while(1);
}
