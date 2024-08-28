/*
 * TestOLED.c
 *
 *  Created on: Aug 27, 2024
 *      Author: Sam
 */

#include "TestFunc/TestFunc.h"
#include "Display/OLEDCtrl.h"
#include "Generic/Activation.h"

#include "Display/ssd1306.h"
#include "Display/ssd1306_tests.h"

static void ShowCurrentParameter(void)
{
	SEGGER_RTT_printf(0, "---> Section of OLED Test Function <---\r\n");
	SEGGER_RTT_printf(0, "-> Click Specific Key to Activate Function\r\n");
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

	OLED_Clear(0);
	OLED_ShowString_11x18W(11, 11, "OLED Test");
    HAL_Delay(300);
	ShowCurrentParameter();

	do
	{
		keyOfPrgs = SEGGER_RTT_GetKey();
		if(keyOfPrgs == '0')
		{
			break;
		}
		else if(keyOfPrgs == '1' || !ReadButton(Button2))
		{
			ssd1306_TestFonts1();
		    HAL_Delay(500);
			OLED_Clear(0);
			OLED_ShowString_11x18W(11, 11, "Show Font1");
			ShowCurrentParameter();
		}
		else if(keyOfPrgs == '2')
		{
			ssd1306_TestFonts2();
		    HAL_Delay(500);
			OLED_Clear(0);
			OLED_ShowString_11x18W(11, 11, "Show Font2");
			ShowCurrentParameter();
		}
		else if(keyOfPrgs == '3')
		{
			ssd1306_TestFonts3();
		    HAL_Delay(500);
			OLED_Clear(0);
			OLED_ShowString_11x18W(11, 11, "Show Font3");
			ShowCurrentParameter();
		}
		else if(keyOfPrgs == '4' || !ReadButton(Button3))
		{
			ssd1306_TestFPS();
		    HAL_Delay(500);
			OLED_Clear(0);
			OLED_ShowString_11x18W(22, 11, "Show FPS");
			ShowCurrentParameter();
		}
		else if(keyOfPrgs == '5' || !ReadButton(Button4))
		{
			OLED_Clear(0);
		    ssd1306_TestCircle();
		    HAL_Delay(500);
			OLED_Clear(0);
			OLED_ShowString_11x18W(0, 11, "Show Circle");
			ShowCurrentParameter();
		}
		else if(keyOfPrgs == '6')
		{
			OLED_Clear(0);
		    ssd1306_TestRectangle();
		    ssd1306_TestLine();
		    HAL_Delay(500);
			OLED_Clear(0);
			OLED_ShowString_11x18W(11, 11, "Show Frame");
			ShowCurrentParameter();
		}
		else if(keyOfPrgs == '7' || !ReadButton(Button5))
		{
			ssd1306_TestDrawBitmap();
		    HAL_Delay(500);
			OLED_Clear(0);
			OLED_ShowString_11x18W(11, 11, "Show Image");
			ShowCurrentParameter();
		}
		else if(keyOfPrgs == '8' || !ReadButton(Button6))
		{
			ssd1306_TestBorder();
		    HAL_Delay(500);
			OLED_Clear(0);
			OLED_ShowString_11x18W(0, 11, "Show Border");
			ShowCurrentParameter();
		}
		else if(keyOfPrgs == '9' || !ReadButton(Button7))
		{
			ssd1306_TestAll();
		    HAL_Delay(500);
			OLED_Clear(0);
			OLED_ShowString_11x18W(11, 11, "Show Multi");
			ShowCurrentParameter();
		}

		if(!ReadButton(Button1))
		{
			OLED_Clear(0);
			OLED_ShowString_16x26W(16, 6, "(OLED)");
		    HAL_Delay(500);
			OLED_Clear(0);
			OLED_ShowString_11x18W(22, 11, "Big Font");
			ShowCurrentParameter();
		}
		else if(!ReadButton(Button8))
		{
			OLED_Clear(0);
			OLED_ShowString_11x18W(22, 11, "(Return)");
		    HAL_Delay(500);
			break;
		}
	}
	while(1);
}
