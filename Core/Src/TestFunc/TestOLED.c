/*
 * TestOLED.c
 *
 *  Created on: Aug 27, 2024
 *      Author: Sam
 */

#include "TestFunc/TestFunc.h"
#include "Display/OLEDCtrl.h"
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

	ShowCurrentParameter();
	OLED_ShowString_11x18("OLED Test");

	do
	{
		keyOfPrgs = SEGGER_RTT_GetKey();
		if(keyOfPrgs == '0')
		{
			break;
		}
		else if(keyOfPrgs == '1')
		{
			ssd1306_TestFonts1();
			ShowCurrentParameter();
		}
		else if(keyOfPrgs == '2')
		{
			ssd1306_TestFonts2();
			ShowCurrentParameter();
		}
		else if(keyOfPrgs == '3')
		{
			ssd1306_TestFonts3();
			ShowCurrentParameter();
		}
		else if(keyOfPrgs == '4')
		{
			ssd1306_TestFPS();
			ShowCurrentParameter();
		}
		else if(keyOfPrgs == '5')
		{
		    ssd1306_Fill(Black);
		    ssd1306_TestCircle();
			ShowCurrentParameter();
		}
		else if(keyOfPrgs == '6')
		{
		    ssd1306_Fill(Black);
		    ssd1306_TestRectangle();
		    ssd1306_TestLine();
			ShowCurrentParameter();
		}
		else if(keyOfPrgs == '7')
		{
			ssd1306_TestDrawBitmap();
			ShowCurrentParameter();
		}
		else if(keyOfPrgs == '8')
		{
			ssd1306_TestBorder();
			ShowCurrentParameter();
		}
		else if(keyOfPrgs == '9')
		{
			ssd1306_TestAll();
			ShowCurrentParameter();
		}
	}
	while(1);
}
