/*
 * TestOLED.c
 *
 *  Created on: Aug 27, 2024
 *      Author: Sam
 */

#include "TestFunc/TestFunc.h"
#include "Display/OLEDCtrl.h"
#include "Generic/Activation.h"
#include "Comm/Postman.h"

#include "Display/ssd1306.h"
#include "Display/ssd1306_tests.h"

static void ShowCurrentParameter(void)
{
	SEGGER_RTT_printf(0, "***********************************************\r\n");
	SEGGER_RTT_printf(0, "* Current Operation: OLED Test                *\r\n");
	SEGGER_RTT_printf(0, "* Click Specific Key to Activate Function.    *\r\n");
	SEGGER_RTT_printf(0, "***********************************************\r\n");
	SEGGER_RTT_printf(0, "-> 1) Show Font1 Test\r\n");
	SEGGER_RTT_printf(0, "-> 2) Show Font2 Test\r\n");
	SEGGER_RTT_printf(0, "-> 3) Show Font3 Test\r\n");
	SEGGER_RTT_printf(0, "-> 4) Show FPS Test\r\n");
	SEGGER_RTT_printf(0, "-> 5) Show Circle Test\r\n");
	SEGGER_RTT_printf(0, "-> 6) Show Rectangle Test\r\n");
	SEGGER_RTT_printf(0, "-> 7) Show Bitmap Test\r\n");
	SEGGER_RTT_printf(0, "-> 8) Show Border Test\r\n");
	SEGGER_RTT_printf(0, "-> 9) Show All Test\r\n");
	SEGGER_RTT_printf(0, "-> 0) Quit\r\n");
}

void TestOledTask(void)
{
	int keyOfPrgs;
	uint8_t virtualBtn;

	OLED_Clear(0);
	OLED_ShowString_11x18W(11, 11, "OLED Test");
    HAL_Delay(300);
	ShowCurrentParameter();

	do
	{
		PostmanTask();
		virtualBtn = GetVirtualButton();
		keyOfPrgs = SEGGER_RTT_GetKey();
		if(keyOfPrgs == '0')
		{
			break;
		}
		else if(keyOfPrgs == '1' || !ReadButton(Button2) || virtualBtn == Button2)
		{
			ssd1306_TestFonts1();
		    HAL_Delay(500);
			OLED_Clear(0);
			OLED_ShowString_11x18W(11, 11, "Show Font1");
			SEGGER_RTT_printf(0, "-> (Show Font1)\r\n");
		}
		else if(keyOfPrgs == '2')
		{
			ssd1306_TestFonts2();
		    HAL_Delay(500);
			OLED_Clear(0);
			OLED_ShowString_11x18W(11, 11, "Show Font2");
			SEGGER_RTT_printf(0, "-> (Show Font2)\r\n");
		}
		else if(keyOfPrgs == '3')
		{
			ssd1306_TestFonts3();
		    HAL_Delay(500);
			OLED_Clear(0);
			OLED_ShowString_11x18W(11, 11, "Show Font3");
			SEGGER_RTT_printf(0, "-> (Show Font3)\r\n");
		}
		else if(keyOfPrgs == '4' || !ReadButton(Button3) || virtualBtn == Button3)
		{
			ssd1306_TestFPS();
		    HAL_Delay(500);
			OLED_Clear(0);
			OLED_ShowString_11x18W(22, 11, "Show FPS");
			SEGGER_RTT_printf(0, "-> (Show FPS)\r\n");
		}
		else if(keyOfPrgs == '5' || !ReadButton(Button4) || virtualBtn == Button4)
		{
			OLED_Clear(0);
		    ssd1306_TestCircle();
		    HAL_Delay(500);
			OLED_Clear(0);
			OLED_ShowString_11x18W(6, 11, "Show Circle");
			SEGGER_RTT_printf(0, "-> (Show Circle)\r\n");
		}
		else if(keyOfPrgs == '6')
		{
			OLED_Clear(0);
		    ssd1306_TestRectangle();
		    ssd1306_TestLine();
		    HAL_Delay(500);
			OLED_Clear(0);
			OLED_ShowString_11x18W(11, 11, "Show Frame");
			SEGGER_RTT_printf(0, "-> (Show Rectangle)\r\n");
		}
		else if(keyOfPrgs == '7' || !ReadButton(Button5) || virtualBtn == Button5)
		{
			ssd1306_TestDrawBitmap();
		    HAL_Delay(500);
			OLED_Clear(0);
			OLED_ShowString_11x18W(11, 11, "Show Image");
			SEGGER_RTT_printf(0, "-> (Show Image)\r\n");
		}
		else if(keyOfPrgs == '8' || !ReadButton(Button6) || virtualBtn == Button6)
		{
			ssd1306_TestBorder();
		    HAL_Delay(500);
			OLED_Clear(0);
			OLED_ShowString_11x18W(0, 11, "Show Border");
			SEGGER_RTT_printf(0, "-> (Show Border)\r\n");
		}
		else if(keyOfPrgs == '9' || !ReadButton(Button7) || virtualBtn == Button7)
		{
			ssd1306_TestAll();
		    HAL_Delay(500);
			OLED_Clear(0);
			OLED_ShowString_11x18W(11, 11, "Show Multi");
			SEGGER_RTT_printf(0, "-> (Show Multiple Tests)\r\n");
		}

		if(!ReadButton(Button1) || virtualBtn == Button1)
		{
			OLED_Clear(0);
			OLED_ShowString_16x26W(16, 6, "(OLED)");
		    HAL_Delay(500);
			OLED_Clear(0);
			OLED_ShowString_11x18W(22, 11, "Big Font");
			SEGGER_RTT_printf(0, "-> (Show Big Font)\r\n");
		}
		else if(!ReadButton(Button8) || virtualBtn == Button8)
		{
			OLED_Clear(0);
			OLED_ShowString_11x18W(22, 11, "(Return)");
			SEGGER_RTT_printf(0, "-> (Return to Main Menu)\r\n\r\n");
		    HAL_Delay(500);
			break;
		}
	}
	while(1);
}
