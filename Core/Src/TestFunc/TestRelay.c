/*
 * TestRelay.c
 *
 *  Created on: Sep 4, 2024
 *      Author: Sam
 */

#include <stdbool.h>

#include "TestFunc/TestFunc.h"
#include "Display/OLEDCtrl.h"
#include "Generic/Encoder.h"
#include "Generic/Activation.h"

static void ConvRelayState(uint16_t sta)
{
	for(int i = 0; i < 9; i++)
	{
		if((sta >> i) & 0x001)
		{
			WriteRelay(Relay9 - i, 1);
		}
		else
		{
			WriteRelay(Relay9 - i, 0);
		}
	}
}

static void ShowRelayCount(uint16_t cnt)
{
	char strCount[10];

	OLED_Clear(0);
	sprintf(strCount, "%d", cnt);
	OLED_ShowString_11x18W(0, 0, "Relay:");
	OLED_ShowString_11x18W(68, 0, strCount);
}

static void ShowButtonTips(uint8_t sel)
{
	if(!sel)
	{
		OLED_ShowString_7x10W(2, 22, "1:Open,2:Up,3:Down");
	}
	else
	{
		OLED_ShowString_7x10W(2, 22, "4:++,5:--,7:Short");
	}
}

static void ShowCurrentParameter(void)
{
	SEGGER_RTT_printf(0, "***********************************************\r\n");
	SEGGER_RTT_printf(0, "* Current Operation: Relay Test               *\r\n");
	SEGGER_RTT_printf(0, "* Click Specific Key to Activate Function.    *\r\n");
	SEGGER_RTT_printf(0, "***********************************************\r\n");
	SEGGER_RTT_printf(0, "-> 0) Quit\r\n\r\n");
	SEGGER_RTT_printf(0, "-> (Control Relays using Encoders or Buttons)\r\n");
}

void TestRelayTask(void)
{
	EncoderParam_t EncTest;

	bool isRelayOpenEnable = false;
	bool isRelayShortEnable = false;
	uint16_t arrRelayState[10] = { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0x1FF };

	int keyOfPrgs;

	LedAllOff();
	RelayAllOff();
	EncoderInit();

	OLED_Clear(0);
	OLED_ShowString_11x18W(11, 11, "Relay Test");
    HAL_Delay(500);
	ShowRelayCount(arrRelayState[0]);
	ShowButtonTips(0);
	ShowCurrentParameter();

	// In this test task, the Relay can be controlled by 2 Encoders or 8 Buttons.
	do
	{
		EncoderTask();
		EncTest = ReadEncoderParam();
		if(!ReadButton(Button1))
		{
			isRelayOpenEnable = !isRelayOpenEnable;
			WriteRelay(RelayOpen, isRelayOpenEnable);
			ShowRelayCount(arrRelayState[EncTest.cntIndex]);
			ShowButtonTips(0);
			WriteEncoderParam(EncTest);
			SEGGER_RTT_printf(0, "-> (Press PS1 Button)\r\n");
		    HAL_Delay(100);
		}
		else if(!ReadButton(Button2))
		{
			EncTest.cntIndex++;
			if(EncTest.cntIndex > 9)
			{
				EncTest.cntIndex = 9;
			}
			ConvRelayState(arrRelayState[EncTest.cntIndex]);
			ShowRelayCount(arrRelayState[EncTest.cntIndex]);
			ShowButtonTips(0);
			EncTest.cntRawValue = arrRelayState[EncTest.cntIndex];
			WriteEncoderParam(EncTest);
			SEGGER_RTT_printf(0, "-> (Press PS2 Button)\r\n");
		    HAL_Delay(100);
		}
		else if(!ReadButton(Button3))
		{
			if(EncTest.cntIndex > 0 && EncTest.cntIndex <= 9)
			{
				EncTest.cntIndex--;
			}
			else
			{
				EncTest.cntIndex = 0;
			}
			ConvRelayState(arrRelayState[EncTest.cntIndex]);
			ShowRelayCount(arrRelayState[EncTest.cntIndex]);
			ShowButtonTips(0);
			EncTest.cntRawValue = arrRelayState[EncTest.cntIndex];
			WriteEncoderParam(EncTest);
			SEGGER_RTT_printf(0, "-> (Press PS3 Button)\r\n");
		    HAL_Delay(100);
		}
		else if(!ReadButton(Button4))
		{
			EncTest.cntRawValue++;
			if(EncTest.cntRawValue > 0x1FF)
			{
				EncTest.cntRawValue = 0x1FF;
			}
			for(int i = 9; i >= 0; i--)
			{
				if(EncTest.cntRawValue >= arrRelayState[i])
				{
					EncTest.cntIndex = i;
					break;
				}
			}
			ConvRelayState(EncTest.cntRawValue);
			ShowRelayCount(EncTest.cntRawValue);
			ShowButtonTips(1);
			WriteEncoderParam(EncTest);
			SEGGER_RTT_printf(0, "-> (Press PS4 Button)\r\n");
		    HAL_Delay(100);
		}
		else if(!ReadButton(Button5))
		{
			if(EncTest.cntRawValue > 0 && EncTest.cntRawValue <= 0x1FF)
			{
				EncTest.cntRawValue--;
			}
			else
			{
				EncTest.cntRawValue = 0;
			}
			for(int i = 9; i >= 0; i--)
			{
				if(EncTest.cntRawValue >= arrRelayState[i])
				{
					EncTest.cntIndex = i;
					break;
				}
			}
			ConvRelayState(EncTest.cntRawValue);
			ShowRelayCount(EncTest.cntRawValue);
			ShowButtonTips(1);
			WriteEncoderParam(EncTest);
			SEGGER_RTT_printf(0, "-> (Press PS5 Button)\r\n");
		    HAL_Delay(100);
		}
		else if(!ReadButton(Button7))
		{
			isRelayShortEnable = !isRelayShortEnable;
			WriteRelay(RelayShort, isRelayShortEnable);
			ShowRelayCount(arrRelayState[EncTest.cntIndex]);
			ShowButtonTips(1);
			WriteEncoderParam(EncTest);
			SEGGER_RTT_printf(0, "-> (Press PS7 Button)\r\n");
		    HAL_Delay(100);
		}
		else if(!ReadButton(Button8))
		{
			LedAllOff();
			RelayAllOff();
			EncoderInit();
			OLED_Clear(0);
			OLED_ShowString_11x18W(22, 11, "(Return)");
			SEGGER_RTT_printf(0, "-> (Press PS8 Button)\r\n\r\n");
		    HAL_Delay(500);
			break;
		}

		keyOfPrgs = SEGGER_RTT_GetKey();
		if(keyOfPrgs == '0')
		{
			LedAllOff();
			RelayAllOff();
			EncoderInit();
			break;
		}
	}
	while(1);
}
