/*
 * TestActivation.c
 *
 *  Created on: Sep 2, 2024
 *      Author: Sam
 */

#include <stdbool.h>

#include "TestFunc/TestFunc.h"
#include "Display/OLEDCtrl.h"
#include "Generic/Activation.h"

typedef void (* GoingFunction)(void);

typedef struct
{
	bool isPS1ButtonEnable;
	bool isPS2ButtonEnable;
	bool isPS3ButtonEnable;
	bool isPS4ButtonEnable;
	bool isPS5ButtonEnable;
	bool isPS6ButtonEnable;
	bool isPS7ButtonEnable;
	bool isPS8ButtonEnable;
	bool isLED1Enable;
	bool isLED2Enable;
	bool isLED3Enable;
	bool isLED4Enable;
	bool isLED5Enable;
	bool isRelay1Enable;
	bool isRelay2Enable;
	bool isRelay3Enable;
	bool isRelay4Enable;
	bool isRelay5Enable;
	bool isRelay6Enable;
	bool isRelay7Enable;
	bool isRelay8Enable;
	bool isRelay9Enable;
	bool isRelayOpenEnable;
	bool isRelayShortEnable;
}TestActParam_t;

static void ShowButtonParameter(TestActParam_t *param);

static void ShowLEDParameter(TestActParam_t *param);

static void ShowRelayParameter(TestActParam_t *param);

void TestActivationButtonTask(void)
{
	TestActParam_t param =
	{
		.isPS1ButtonEnable = false,
		.isPS2ButtonEnable = false,
		.isPS3ButtonEnable = false,
		.isPS4ButtonEnable = false,
		.isPS5ButtonEnable = false,
		.isPS6ButtonEnable = false,
		.isPS7ButtonEnable = false,
		.isPS8ButtonEnable = false,
	};

	int keyOfPrgs;

	OLED_Clear(0);
	OLED_ShowString_11x18W(6, 11, "Set Button");
    HAL_Delay(300);
	ShowButtonParameter(&param);

	do
	{
		keyOfPrgs = SEGGER_RTT_GetKey();

		if( keyOfPrgs == '1' || keyOfPrgs == '2' || keyOfPrgs == '3' || keyOfPrgs == '4' ||
			keyOfPrgs == '5' || keyOfPrgs == '6' || keyOfPrgs == '7' || keyOfPrgs == '8')
		{
			param.isPS1ButtonEnable = ReadButton(Button1)? false : true;
			param.isPS2ButtonEnable = ReadButton(Button2)? false : true;
			param.isPS3ButtonEnable = ReadButton(Button3)? false : true;
			param.isPS4ButtonEnable = ReadButton(Button4)? false : true;
			param.isPS5ButtonEnable = ReadButton(Button5)? false : true;
			param.isPS6ButtonEnable = ReadButton(Button6)? false : true;
			param.isPS7ButtonEnable = ReadButton(Button7)? false : true;
			param.isPS8ButtonEnable = ReadButton(Button8)? false : true;
			ShowButtonParameter(&param);
		}
		else if(keyOfPrgs == '0')
		{
			break;
		}
	}
	while(1);
}

void TestActivationLedTask(void)
{
	TestActParam_t param =
	{
		.isLED1Enable = false,
		.isLED2Enable = false,
		.isLED3Enable = false,
		.isLED4Enable = false,
		.isLED5Enable = false,
	};

	int keyOfPrgs;

	LedAllOff();
	OLED_Clear(0);
	OLED_ShowString_11x18W(16, 11, "Set LED");
    HAL_Delay(300);
	ShowLEDParameter(&param);

	do
	{
		keyOfPrgs = SEGGER_RTT_GetKey();

		if(keyOfPrgs == '1')
		{
			param.isLED1Enable = !param.isLED1Enable;
			WriteLED(LED1, param.isLED1Enable);
			ShowLEDParameter(&param);
		}
		else if(keyOfPrgs == '2')
		{
			param.isLED2Enable = !param.isLED2Enable;
			WriteLED(LED2, param.isLED2Enable);
			ShowLEDParameter(&param);
		}
		else if(keyOfPrgs == '3')
		{
			param.isLED3Enable = !param.isLED3Enable;
			WriteLED(LED3, param.isLED3Enable);
			ShowLEDParameter(&param);
		}
		else if(keyOfPrgs == '4')
		{
			param.isLED4Enable = !param.isLED4Enable;
			WriteLED(LED4, param.isLED4Enable);
			ShowLEDParameter(&param);
		}
		else if(keyOfPrgs == '5')
		{
			param.isLED5Enable = !param.isLED5Enable;
			WriteLED(LED5, param.isLED5Enable);
			ShowLEDParameter(&param);
		}
		else if(keyOfPrgs == '0')
		{
			LedAllOff();
			break;
		}
	}
	while(1);
}

void TestActivationRelayTask(void)
{
	TestActParam_t param =
	{
		.isRelay1Enable = false,
		.isRelay2Enable = false,
		.isRelay3Enable = false,
		.isRelay4Enable = false,
		.isRelay5Enable = false,
		.isRelay6Enable = false,
		.isRelay7Enable = false,
		.isRelay8Enable = false,
		.isRelay9Enable = false,
		.isRelayOpenEnable = false,
		.isRelayShortEnable = false,
	};

	int keyOfPrgs;

	RelayAllOff();
	OLED_Clear(0);
	OLED_ShowString_11x18W(16, 11, "Set Relay");
    HAL_Delay(300);
	ShowRelayParameter(&param);

	do
	{
		keyOfPrgs = SEGGER_RTT_GetKey();

		if(keyOfPrgs == '1')
		{
			param.isRelay1Enable = !param.isRelay1Enable;
			WriteRelay(Relay1, param.isRelay1Enable);
			ShowRelayParameter(&param);
		}
		else if(keyOfPrgs == '2')
		{
			param.isRelay2Enable = !param.isRelay2Enable;
			WriteRelay(Relay2, param.isRelay2Enable);
			ShowRelayParameter(&param);
		}
		else if(keyOfPrgs == '3')
		{
			param.isRelay3Enable = !param.isRelay3Enable;
			WriteRelay(Relay3, param.isRelay3Enable);
			ShowRelayParameter(&param);
		}
		else if(keyOfPrgs == '4')
		{
			param.isRelay4Enable = !param.isRelay4Enable;
			WriteRelay(Relay4, param.isRelay4Enable);
			ShowRelayParameter(&param);
		}
		else if(keyOfPrgs == '5')
		{
			param.isRelay5Enable = !param.isRelay5Enable;
			WriteRelay(Relay5, param.isRelay5Enable);
			ShowRelayParameter(&param);
		}
		else if(keyOfPrgs == '6')
		{
			param.isRelay6Enable = !param.isRelay6Enable;
			WriteRelay(Relay6, param.isRelay6Enable);
			ShowRelayParameter(&param);
		}
		else if(keyOfPrgs == '7')
		{
			param.isRelay7Enable = !param.isRelay7Enable;
			WriteRelay(Relay7, param.isRelay7Enable);
			ShowRelayParameter(&param);
		}
		else if(keyOfPrgs == '8')
		{
			param.isRelay8Enable = !param.isRelay8Enable;
			WriteRelay(Relay8, param.isRelay8Enable);
			ShowRelayParameter(&param);
		}
		else if(keyOfPrgs == '9')
		{
			param.isRelay9Enable = !param.isRelay9Enable;
			WriteRelay(Relay9, param.isRelay9Enable);
			ShowRelayParameter(&param);
		}
		else if(keyOfPrgs == '+')
		{
			param.isRelay1Enable = true;
			WriteRelay(Relay1, param.isRelay1Enable);
			param.isRelay2Enable = true;
			WriteRelay(Relay2, param.isRelay2Enable);
			param.isRelay3Enable = true;
			WriteRelay(Relay3, param.isRelay3Enable);
			param.isRelay4Enable = true;
			WriteRelay(Relay4, param.isRelay4Enable);
			param.isRelay5Enable = true;
			WriteRelay(Relay5, param.isRelay5Enable);
			param.isRelay6Enable = true;
			WriteRelay(Relay6, param.isRelay6Enable);
			param.isRelay7Enable = true;
			WriteRelay(Relay7, param.isRelay7Enable);
			param.isRelay8Enable = true;
			WriteRelay(Relay8, param.isRelay8Enable);
			param.isRelay9Enable = true;
			WriteRelay(Relay9, param.isRelay9Enable);
			ShowRelayParameter(&param);
		}
		else if(keyOfPrgs == '-')
		{
			param.isRelay1Enable = false;
			WriteRelay(Relay1, param.isRelay1Enable);
			param.isRelay2Enable = false;
			WriteRelay(Relay2, param.isRelay2Enable);
			param.isRelay3Enable = false;
			WriteRelay(Relay3, param.isRelay3Enable);
			param.isRelay4Enable = false;
			WriteRelay(Relay4, param.isRelay4Enable);
			param.isRelay5Enable = false;
			WriteRelay(Relay5, param.isRelay5Enable);
			param.isRelay6Enable = false;
			WriteRelay(Relay6, param.isRelay6Enable);
			param.isRelay7Enable = false;
			WriteRelay(Relay7, param.isRelay7Enable);
			param.isRelay8Enable = false;
			WriteRelay(Relay8, param.isRelay8Enable);
			param.isRelay9Enable = false;
			WriteRelay(Relay9, param.isRelay9Enable);
			ShowRelayParameter(&param);
		}
		else if(keyOfPrgs == '*')
		{
			param.isRelayOpenEnable = !param.isRelayOpenEnable;
			WriteRelay(RelayOpen, param.isRelayOpenEnable);
			ShowRelayParameter(&param);
		}
		else if(keyOfPrgs == '/')
		{
			param.isRelayShortEnable = !param.isRelayShortEnable;
			WriteRelay(RelayShort, param.isRelayShortEnable);
			ShowRelayParameter(&param);
		}
		else if(keyOfPrgs == '0')
		{
			RelayAllOff();
			break;
		}
	}
	while(1);
}

static void ShowButtonParameter(TestActParam_t *param)
{
	SEGGER_RTT_printf(0, "***********************************************\r\n");
	SEGGER_RTT_printf(0, "* Current Operation: Get Button Status        *\r\n");
	SEGGER_RTT_printf(0, "* Click Specific Key to Activate Function.    *\r\n");
	SEGGER_RTT_printf(0, "***********************************************\r\n");
	SEGGER_RTT_printf(0, "-> '1') Get PS1 Button (State: %s)\r\n", param->isPS1ButtonEnable ? "Low" : "High");
	SEGGER_RTT_printf(0, "-> '2') Get PS2 Button (State: %s)\r\n", param->isPS2ButtonEnable ? "Low" : "High");
	SEGGER_RTT_printf(0, "-> '3') Get PS3 Button (State: %s)\r\n", param->isPS3ButtonEnable ? "Low" : "High");
	SEGGER_RTT_printf(0, "-> '4') Get PS4 Button (State: %s)\r\n", param->isPS4ButtonEnable ? "Low" : "High");
	SEGGER_RTT_printf(0, "-> '5') Get PS5 Button (State: %s)\r\n", param->isPS5ButtonEnable ? "Low" : "High");
	SEGGER_RTT_printf(0, "-> '6') Get PS6 Button (State: %s)\r\n", param->isPS6ButtonEnable ? "Low" : "High");
	SEGGER_RTT_printf(0, "-> '7') Get PS7 Button (State: %s)\r\n", param->isPS7ButtonEnable ? "Low" : "High");
	SEGGER_RTT_printf(0, "-> '8') Get PS8 Button (State: %s)\r\n", param->isPS8ButtonEnable ? "Low" : "High");
	SEGGER_RTT_printf(0, "-> '0') Quit\r\n");
}

static void ShowLEDParameter(TestActParam_t *param)
{
	SEGGER_RTT_printf(0, "***********************************************\r\n");
	SEGGER_RTT_printf(0, "* Current Operation: Set LED On/Off           *\r\n");
	SEGGER_RTT_printf(0, "* Click Specific Key to Activate Function.    *\r\n");
	SEGGER_RTT_printf(0, "***********************************************\r\n");
	SEGGER_RTT_printf(0, "-> '1') Set LD_1 (State: %s)\r\n", param->isLED1Enable ? "On" : "Off");
	SEGGER_RTT_printf(0, "-> '2') Set LD_2 (State: %s)\r\n", param->isLED2Enable ? "On" : "Off");
	SEGGER_RTT_printf(0, "-> '3') Set LD_3 (State: %s)\r\n", param->isLED3Enable ? "On" : "Off");
	SEGGER_RTT_printf(0, "-> '4') Set LD_4 (State: %s)\r\n", param->isLED4Enable ? "On" : "Off");
	SEGGER_RTT_printf(0, "-> '5') Set LD_5 (State: %s)\r\n", param->isLED5Enable ? "On" : "Off");
	SEGGER_RTT_printf(0, "-> '0') Quit\r\n");
}

static void ShowRelayParameter(TestActParam_t *param)
{
	SEGGER_RTT_printf(0, "***********************************************\r\n");
	SEGGER_RTT_printf(0, "* Current Operation: Set Relay On/Off         *\r\n");
	SEGGER_RTT_printf(0, "* Click Specific Key to Activate Function.    *\r\n");
	SEGGER_RTT_printf(0, "***********************************************\r\n");
	SEGGER_RTT_printf(0, "-> '1') Set Relay_1 (State: %s)\r\n", param->isRelay1Enable ? "On" : "Off");
	SEGGER_RTT_printf(0, "-> '2') Set Relay_2 (State: %s)\r\n", param->isRelay2Enable ? "On" : "Off");
	SEGGER_RTT_printf(0, "-> '3') Set Relay_3 (State: %s)\r\n", param->isRelay3Enable ? "On" : "Off");
	SEGGER_RTT_printf(0, "-> '4') Set Relay_4 (State: %s)\r\n", param->isRelay4Enable ? "On" : "Off");
	SEGGER_RTT_printf(0, "-> '5') Set Relay_5 (State: %s)\r\n", param->isRelay5Enable ? "On" : "Off");
	SEGGER_RTT_printf(0, "-> '6') Set Relay_6 (State: %s)\r\n", param->isRelay6Enable ? "On" : "Off");
	SEGGER_RTT_printf(0, "-> '7') Set Relay_7 (State: %s)\r\n", param->isRelay7Enable ? "On" : "Off");
	SEGGER_RTT_printf(0, "-> '8') Set Relay_8 (State: %s)\r\n", param->isRelay8Enable ? "On" : "Off");
	SEGGER_RTT_printf(0, "-> '9') Set Relay_9 (State: %s)\r\n", param->isRelay9Enable ? "On" : "Off");
	SEGGER_RTT_printf(0, "-> '+') Set Relay 1~9 All On\r\n");
	SEGGER_RTT_printf(0, "-> '-') Set Relay 1~9 All Off\r\n");
	SEGGER_RTT_printf(0, "-> '*') Set Relay_Open (State: %s)\r\n", param->isRelayOpenEnable ? "On" : "Off");
	SEGGER_RTT_printf(0, "-> '/') Set Relay_Short (State: %s)\r\n", param->isRelayShortEnable ? "On" : "Off");
	SEGGER_RTT_printf(0, "-> '0') Quit\r\n");
}
