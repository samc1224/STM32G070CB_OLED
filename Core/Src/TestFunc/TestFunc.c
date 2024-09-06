/*
 * TestFunc.c
 *
 *  Created on: Aug 27, 2024
 *      Author: Sam
 */

#include <stdbool.h>
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
	static bool isMainMenu = false;

	switch(taskSeq)
	{
		case TestFuncTaskSeq_ShowHintTitle:
			if(isMainMenu)
			{
				SEGGER_RTT_printf(0, "***********************************************\r\n");
				SEGGER_RTT_printf(0, "* Current Operation: Test Mode                *\r\n");
				SEGGER_RTT_printf(0, "* Click Specific Key to launch Test Function. *\r\n");
				SEGGER_RTT_printf(0, "***********************************************\r\n");
				SEGGER_RTT_printf(0, "-> Click '1' > Relay Function Test\r\n");
				SEGGER_RTT_printf(0, "-> Click '2' > OLED Function Test\r\n");
				SEGGER_RTT_printf(0, "-> Click '3' > Button & LED Function Test\r\n");
				SEGGER_RTT_printf(0, "-> Click '4' > Get Button Status\r\n");
				SEGGER_RTT_printf(0, "-> Click '5' > Set LED Action\r\n");
				SEGGER_RTT_printf(0, "-> Click '6' > Set Relay Action\r\n");
				SEGGER_RTT_printf(0, "-> Click '0' > Switch to Encoder Mode\r\n");
				OLED_Clear(0);
				OLED_ShowString_11x18W(0, 0, "Test Mode");
				OLED_ShowString_7x10W(2, 22, "1:Relay, 2:OLED...");
				//
				LedAllOff();
				RelayAllOff();
				EncoderInit();
				SetEncoderResistorConversion(false);
			}
			else
			{
				SEGGER_RTT_printf(0, "***********************************************\r\n");
				SEGGER_RTT_printf(0, "* Current Operation: Encoder Mode             *\r\n");
				SEGGER_RTT_printf(0, "* Click Specific Key to Activate Function.    *\r\n");
				SEGGER_RTT_printf(0, "***********************************************\r\n");
				SEGGER_RTT_printf(0, "-> Click '1' > Set Encoder Small Multiple (50/100 ohm)\r\n");
				SEGGER_RTT_printf(0, "-> Click '2' > Set Encoder Big Multiple (1k/2k ohm)\r\n");
				SEGGER_RTT_printf(0, "-> Click '0' > Switch to Test Mode\r\n\r\n");
				SEGGER_RTT_printf(0, "-> (Encoders are available to control Relays)\r\n");
				OLED_Clear(0);
				OLED_ShowString_11x18W(0, 11, "Encoder Act");
				//
				LedAllOff();
				RelayAllOff();
				EncoderInit();
				SetEncoderCountRawValue(0x1FF);
				WriteRelay(RelayOpen, 1);
				SetEncoderResistorConversion(true);
			}
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
				/*
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
				*/
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
					isMainMenu = !isMainMenu;
					if(isMainMenu)
					{
						SEGGER_RTT_printf(0, "(Switch to Test Mode)\r\n\r\n");
					}
					else
					{
						SEGGER_RTT_printf(0, "(Switch to Encoder Act)\r\n\r\n");
					}
					taskSeq = TestFuncTaskSeq_ShowHintTitle;
					break;
			}

			if(!isMainMenu)
			{
				EncoderTask();
			}
			if(!ReadButton(Button1))
			{
				if(isMainMenu)
				{
					testFunc = TestRelayTask;
					taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
				}
				else
				{
					ChangeEncoderSmallMultiple();
					HAL_Delay(500);
					taskSeq = TestFuncTaskSeq_GetTestSeries;
				}
			}
			else if(!ReadButton(Button2))
			{
				if(isMainMenu)
				{
					testFunc = TestOledTask;
					taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
				}
				else
				{
					ChangeEncoderBigMultiple();
				    HAL_Delay(500);
					taskSeq = TestFuncTaskSeq_GetTestSeries;
				}
			}
			else if(!ReadButton(Button3))
			{
				testFunc = TestButtonTask;
				taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
			}
			else if(!ReadButton(Button7))
			{
				if(isMainMenu)
				{
					isMainMenu = true;
					ShowYellowStone();
					SEGGER_RTT_printf(0, "(Return to Test Mode)\r\n");
					SEGGER_RTT_printf(0, "(OLED Show \"YellowStone\")\r\n\r\n");
					taskSeq = TestFuncTaskSeq_ShowHintTitle;
				}
				else
				{
				    HAL_Delay(300);
				    if(!ReadRelay(RelayShort))
				    {
				    	WriteRelay(RelayShort, 1);
				    }
				    else
				    {
				    	WriteRelay(RelayShort, 0);
				    }
					SEGGER_RTT_printf(0, "(Press PS7 Button)\r\n");
					taskSeq = TestFuncTaskSeq_GetTestSeries;
				}
			}
			else if(!ReadButton(Button8))
			{
				isMainMenu = !isMainMenu;
			    HAL_Delay(300);
				if(isMainMenu)
				{
					SEGGER_RTT_printf(0, "(Switch to Test Mode)\r\n\r\n");
				}
				else
				{
					SEGGER_RTT_printf(0, "(Switch to Encoder Act)\r\n\r\n");
				}
				taskSeq = TestFuncTaskSeq_ShowHintTitle;
			}
			else if(!ReadButton(ButtonSe1))
			{
				ChangeEncoderSmallMultiple();
			    HAL_Delay(500);
				taskSeq = TestFuncTaskSeq_GetTestSeries;
			}
			else if(!ReadButton(ButtonSe2))
			{
				ChangeEncoderBigMultiple();
			    HAL_Delay(500);
				taskSeq = TestFuncTaskSeq_GetTestSeries;
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
