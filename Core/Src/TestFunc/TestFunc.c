/*
 * TestFunc.c
 *
 *  Created on: Aug 27, 2024
 *      Author: Sam
 */

#include "main.h"
#include "TestFunc/TestFunc.h"
#include "Display/OLEDCtrl.h"
#include "Generic/Activation.h"

typedef enum
{
	TestFuncTaskSeq_ShowHintTitle,
	TestFuncTaskSeq_GetTestSeries,
	TestFuncTaskSeq_ExecuteTestFunc,
	TestFuncTaskSeq_Exit
}TestFuncTaskSeq;

/* Entry of Test Function */
typedef void (* TestFunctionEntry)(void);

void ShowYellowStone()
{
	char strYst[] = {"YellowStone"};

	//OLED_ShowString_11x18W(6, 7, "YellowStone", 1);
	OLED_Clear(0);
	for(int i = 0; i < 11; i++)
	{
		OLED_ShowChar_11x18W(6+(i*11), 7, strYst[i]);
	    HAL_Delay(50);
	}
    HAL_Delay(500);
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
			SEGGER_RTT_printf(0, "-> Click '1' > OLED Test Function\r\n");
			SEGGER_RTT_printf(0, "-> Click '2' > Button & LED Test Function\r\n");
			SEGGER_RTT_printf(0, "-> Click '3' > Encoder Test Function\r\n");
			SEGGER_RTT_printf(0, "-> Click '4' > Get Button Status\r\n");
			SEGGER_RTT_printf(0, "-> Click '5' > Set LED On/Off\r\n");
			SEGGER_RTT_printf(0, "-> Click '6' > Set Relay On/Off\r\n");
			SEGGER_RTT_printf(0, "-> Click '0' > Title\r\n");
			OLED_Clear(0);
			OLED_ShowString_11x18W(0, 0, "Main Menu");
			OLED_ShowString_7x10W(2, 22, "1->OLED, 2->Button");
			//
			taskSeq = TestFuncTaskSeq_GetTestSeries;
			break;
		case TestFuncTaskSeq_GetTestSeries:
			switch(SEGGER_RTT_GetKey())
			{
				case '1':
					testFunc = TestOledTask;
					taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
					break;
				case '2':
					testFunc = TestButtonTask;
					taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
					break;
				case '3':
					SEGGER_RTT_printf(0, "(Encoder Testing...)\r\n\r\n");
					SEGGER_RTT_printf(0, "(Forward: Count Up, Reverse: Count Down)\r\n");
					OLED_Clear(0);
					OLED_ShowString_11x18W(0, 11, "EncoderTest");
					taskSeq = TestFuncTaskSeq_GetTestSeries;
					break;
				case '4':
					testFunc = TestActivationButtonTask;
					taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
					break;
				case '5':
					testFunc = TestActivationLedTask;
					taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
					break;
				case '6':
					testFunc = TestActivationRelayTask;
					taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
					break;
				case '0':
					SEGGER_RTT_printf(0, "(Return to Main Menu)\r\n");
					SEGGER_RTT_printf(0, "(OLED Show \"YellowStone\")\r\n\r\n");
					ShowYellowStone();
					taskSeq = TestFuncTaskSeq_ShowHintTitle;
					break;
			}

			if(!ReadButton(Button1))
			{
				testFunc = TestOledTask;
				taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
			}
			else if(!ReadButton(Button2))
			{
				testFunc = TestButtonTask;
				taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
			}
			else if(!ReadButton(Button3))
			{
				SEGGER_RTT_printf(0, "(Encoder Testing...)\r\n\r\n");
				SEGGER_RTT_printf(0, "(Forward: LED On, Reverse: LED Off)\r\n");
				OLED_Clear(0);
				OLED_ShowString_11x18W(0, 11, "EncoderTest");
			}
			else if(!ReadButton(Button7))
			{
				ShowYellowStone();
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
