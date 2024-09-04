/*
 * TestFunc.c
 *
 *  Created on: Aug 27, 2024
 *      Author: Sam
 */

#include "TestFunc/TestFunc.h"
#include "Display/OLEDCtrl.h"
#include "Generic/Activation.h"
#include "Generic/Encoder.h"

typedef enum
{
	TestFuncTaskSeq_ShowHintTitle,
	TestFuncTaskSeq_GetTestSeries,
	TestFuncTaskSeq_ExecuteTestFunc,
	TestFuncTaskSeq_Exit
}TestFuncTaskSeq;

/* Entry of Test Function */
typedef void (* TestFunctionEntry)(void);

static void ShowYellowStone()
{
	char strYst[] = {"YellowStone"};

	OLED_Clear(0);
	for(int i = 0; i < 11; i++)
	{
		OLED_ShowChar_11x18W(6+(i*11), 7, strYst[i]);
	    HAL_Delay(50);
	}
    HAL_Delay(300);
}

PrcsRes TestFuncTask(void)
{
	static TestFuncTaskSeq taskSeq = TestFuncTaskSeq_ShowHintTitle;
	static TestFunctionEntry testFunc = NULL;

	switch(taskSeq)
	{
		case TestFuncTaskSeq_ShowHintTitle:
			SEGGER_RTT_printf(0, "***********************************************\r\n");
			SEGGER_RTT_printf(0, "* You are now enter into Test Mode.           *\r\n");
			SEGGER_RTT_printf(0, "* Enter Test KeyWord to launch Test Function. *\r\n\r\n");
			//
			SEGGER_RTT_printf(0, "-> Click '1' > Relay Function Test\r\n");
			SEGGER_RTT_printf(0, "-> Click '2' > OLED Function Test\r\n");
			SEGGER_RTT_printf(0, "-> Click '3' > Button & LED Function Test\r\n");
			SEGGER_RTT_printf(0, "-> Click '4' > Encoder Function Test\r\n");
			SEGGER_RTT_printf(0, "-> Click '5' > Get Button Status\r\n");
			SEGGER_RTT_printf(0, "-> Click '6' > Set LED On/Off\r\n");
			SEGGER_RTT_printf(0, "-> Click '0' > Return\r\n\r\n");
			OLED_Clear(0);
			OLED_ShowString_11x18W(0, 0, "Main Menu");
			OLED_ShowString_7x10W(2, 22, "1:Relay, 2:OLED...");
			//
			taskSeq = TestFuncTaskSeq_GetTestSeries;
			break;
		case TestFuncTaskSeq_GetTestSeries:
			switch(SEGGER_RTT_GetKey())
			{
				case '1':
					testFunc = TestRelayTask;
					taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
					break;
				case '2':
					testFunc = TestOledTask;
					taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
					break;
				case '3':
					testFunc = TestButtonTask;
					taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
					break;
				case '4':
					LedAllOff();
					RelayAllOff();
					EncoderInit();
					OLED_Clear(0);
					OLED_ShowString_11x18W(6, 11, "EncoderTest");
					SEGGER_RTT_printf(0, "(Encoder Testing...)\r\n");
					SEGGER_RTT_printf(0, "(Forward: Count Up, Reverse: Count Down)\r\n\r\n");
					taskSeq = TestFuncTaskSeq_GetTestSeries;
					break;
				case '5':
					testFunc = TestActivationButtonTask;
					taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
					break;
				case '6':
					testFunc = TestActivationLedTask;
					taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
					break;
				case '0':
					ShowYellowStone();
					SEGGER_RTT_printf(0, "(Return to Main Menu)\r\n");
					SEGGER_RTT_printf(0, "(OLED Show \"YellowStone\")\r\n\r\n");
					taskSeq = TestFuncTaskSeq_ShowHintTitle;
					break;
			}

			if(!ReadButton(Button1))
			{
				testFunc = TestRelayTask;
				taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
			}
			else if(!ReadButton(Button2))
			{
				testFunc = TestOledTask;
				taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
			}
			else if(!ReadButton(Button3))
			{
				testFunc = TestButtonTask;
				taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
			}
			else if(!ReadButton(Button4))
			{
				LedAllOff();
				RelayAllOff();
				EncoderInit();
				OLED_Clear(0);
				OLED_ShowString_11x18W(6, 11, "EncoderTest");
				SEGGER_RTT_printf(0, "(Encoder Testing...)\r\n");
				SEGGER_RTT_printf(0, "(Forward: Count Up, Reverse: Count Down)\r\n\r\n");
			}
			else if(!ReadButton(Button7))
			{
				ShowYellowStone();
				SEGGER_RTT_printf(0, "(Return to Main Menu)\r\n");
				SEGGER_RTT_printf(0, "(OLED Show \"YellowStone\")\r\n\r\n");
				taskSeq = TestFuncTaskSeq_ShowHintTitle;
			}
			else if(!ReadButton(Button8))
			{
				taskSeq = TestFuncTaskSeq_ShowHintTitle;
			}
			break;
		case TestFuncTaskSeq_ExecuteTestFunc:
			if(testFunc == NULL)
			{
				taskSeq = TestFuncTaskSeq_ShowHintTitle;
				break;
			}
			/* Execute Specific Test Function */
			testFunc();
			taskSeq = TestFuncTaskSeq_ShowHintTitle;
			break;
		case TestFuncTaskSeq_Exit:
			/* Not Possible to Here */
			return PrcsRes_Success;
	}
	//
	return PrcsRes_Processing;
}
