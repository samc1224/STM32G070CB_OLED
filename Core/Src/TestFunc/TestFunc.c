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

static EncoderParam_t EncTestParam;

static void ShowScrollYellowStone()
{
	char strYst[] = {"YellowStone"};

	OLED_Clear(0);
	for(int i = 0; i < 11; i++)
	{
		OLED_ShowChar_11x18W(6+(i*11), 7, strYst[i]);
	    HAL_Delay(50);
	}
    HAL_Delay(300);
	OLED_ScrollClear(0);
}

static void PressPS1Button()
{
	LedAllOff();
	WriteLED(LED3, 1);
	SetEncoderCountRawValue(20); // 20 * 50 = 1000
	ConvertResistorValueToRelay(20);
	ShowEncoderCount();
	SEGGER_RTT_printf(0, "(Press PS1 Button)\r\n");
}

static void PressPS2Button()
{
	LedAllOff();
	WriteLED(LED4, 1);
	SetEncoderCountRawValue(120); // 120 * 50 = 6000
	ConvertResistorValueToRelay(120);
	ShowEncoderCount();
	SEGGER_RTT_printf(0, "(Press PS2 Button)\r\n");
}

static void PressPS3Button()
{
	LedAllOff();
	WriteLED(LED5, 1);
	SetEncoderCountRawValue(180); // 180 * 50 = 9000
	ConvertResistorValueToRelay(180);
	ShowEncoderCount();
	SEGGER_RTT_printf(0, "(Press PS3 Button)\r\n");
}

static void PressPS4Button()
{
	LedAllOff();
	WriteLED(LED2, 1);
	SetEncoderCountRawValue(200); // 200 * 50 = 10000
	ConvertResistorValueToRelay(200);
	ShowEncoderCount();
	SEGGER_RTT_printf(0, "(Press PS4 Button)\r\n");
}

static void PressPS5Button()
{
	LedAllOff();
	WriteLED(LED1, 1);
	SetEncoderCountRawValue(511); // 511 * 50 = 25550
	ConvertResistorValueToRelay(511);
	ShowEncoderCount();
	SEGGER_RTT_printf(0, "(Press PS5 Button)\r\n");
}

static void PressPS6Button()
{
	EncTestParam.cntSmallMultiple = !EncTestParam.cntSmallMultiple;
	ChangeEncoderSmallMultiple(EncTestParam.cntSmallMultiple);
	HAL_Delay(500);
	SEGGER_RTT_printf(0, "(Press PS6 Button)\r\n");
}

static void PressPS7Button()
{
	EncTestParam.cntBigMultiple = !EncTestParam.cntBigMultiple;
	ChangeEncoderBigMultiple(EncTestParam.cntBigMultiple);
    HAL_Delay(500);
	SEGGER_RTT_printf(0, "(Press PS7 Button)\r\n");
}

PrcsRes TestFuncTask(void)
{
	static TestFuncTaskSeq taskSeq = TestFuncTaskSeq_ShowHintTitle;
	static TestFunctionEntry testFunc = NULL;
	static bool isMainMenu = false; // true: Test Mode, false: Encoder Mode

	uint8_t virtualBtn;

	switch(taskSeq)
	{
		case TestFuncTaskSeq_ShowHintTitle:
			if(isMainMenu)
			{
				SEGGER_RTT_printf(0, "***********************************************\r\n");
				SEGGER_RTT_printf(0, "* Current Operation: Test Mode                *\r\n");
				SEGGER_RTT_printf(0, "* I2C Address Setting: 0x%X                   *\r\n", I2C_ADDR);
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
				SEGGER_RTT_printf(0, "* I2C Address Setting: 0x%X                   *\r\n", I2C_ADDR);
				SEGGER_RTT_printf(0, "* Click Specific Key to Activate Function.    *\r\n");
				SEGGER_RTT_printf(0, "***********************************************\r\n");
				SEGGER_RTT_printf(0, "-> Click '1' > Set Resistance to 1K Ohm\r\n");
				SEGGER_RTT_printf(0, "-> Click '2' > Set Resistance to 6K Ohm\r\n");
				SEGGER_RTT_printf(0, "-> Click '3' > Set Resistance to 9K Ohm\r\n");
				SEGGER_RTT_printf(0, "-> Click '4' > Set Resistance to 10K Ohm\r\n");
				SEGGER_RTT_printf(0, "-> Click '5' > Set Resistance to 25.5K Ohm\r\n");
				SEGGER_RTT_printf(0, "-> Click '6' > Set Encoder Small Scale (50/100 Ohm)\r\n");
				SEGGER_RTT_printf(0, "-> Click '7' > Set Encoder Large Scale (1K/2K Ohm)\r\n");
				SEGGER_RTT_printf(0, "-> Click '0' > Switch to Test Mode\r\n\r\n");
				SEGGER_RTT_printf(0, "-> (Encoders are available to control Relays)\r\n");
				OLED_Clear(0);
				OLED_ShowString_11x18W(0, 0, "Encoder Act");
				OLED_ShowString_7x10W(2, 22, "1K, 6K, 9K, 10K...)");
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
			if(!isMainMenu)
			{
				EncoderTask();
			}
			//
			virtualBtn = GetVirtualButton();
			EncTestParam = ReadEncoderParam();
			if(!ReadButton(Button1) || virtualBtn == Button1)
			{
				if(isMainMenu) // Test Mode
				{
					testFunc = TestRelayTask;
					taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
				}
				else // Encoder Mode
				{
					PressPS1Button();
				}
			}
			else if(!ReadButton(Button2) || virtualBtn == Button2)
			{
				if(isMainMenu) // Test Mode
				{
					testFunc = TestOledTask;
					taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
				}
				else // Encoder Mode
				{
					PressPS2Button();
				}
			}
			else if(!ReadButton(Button3) || virtualBtn == Button3)
			{
				if(isMainMenu) // Test Mode
				{
					testFunc = TestButtonTask;
					taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
				}
				else // Encoder Mode
				{
					PressPS3Button();
				}
			}
			else if(!ReadButton(Button4) || virtualBtn == Button4)
			{
				if(!isMainMenu) // Encoder Mode
				{
					PressPS4Button();
				}
			}
			else if(!ReadButton(Button5) || virtualBtn == Button5)
			{
				if(!isMainMenu) // Encoder Mode
				{
					PressPS5Button();
				}
			}
			else if(!ReadButton(Button6) || virtualBtn == Button6)
			{
				if(!isMainMenu) // Encoder Mode
				{
					PressPS6Button();
				}
			}
			else if(!ReadButton(Button7) || virtualBtn == Button7)
			{
				if(isMainMenu) // Test Mode
				{
					ShowScrollYellowStone();
					SEGGER_RTT_printf(0, "(Return to Test Mode)\r\n");
					SEGGER_RTT_printf(0, "(OLED Show \"YellowStone\")\r\n\r\n");
					taskSeq = TestFuncTaskSeq_ShowHintTitle;
				}
				else // Encoder Mode
				{
					PressPS7Button();
				}
			}
			else if(!ReadButton(Button8) || virtualBtn == Button8)
			{
				isMainMenu = !isMainMenu;
			    HAL_Delay(300);
				if(isMainMenu) // Test Mode
				{
					SEGGER_RTT_printf(0, "(Switch to Test Mode)\r\n\r\n");
				}
				else // Encoder Mode
				{
					SEGGER_RTT_printf(0, "(Switch to Encoder Mode)\r\n\r\n");
				}
				taskSeq = TestFuncTaskSeq_ShowHintTitle;
			}
			else if(!ReadButton(ButtonSe1) || virtualBtn == ButtonSe1)
			{
				EncTestParam.cntSmallMultiple = !EncTestParam.cntSmallMultiple;
				ChangeEncoderSmallMultiple(EncTestParam.cntSmallMultiple);
			    HAL_Delay(500);
				taskSeq = TestFuncTaskSeq_GetTestSeries;
			}
			else if(!ReadButton(ButtonSe2) || virtualBtn == ButtonSe2)
			{
				EncTestParam.cntBigMultiple = !EncTestParam.cntBigMultiple;
				ChangeEncoderBigMultiple(EncTestParam.cntBigMultiple);
			    HAL_Delay(500);
				taskSeq = TestFuncTaskSeq_GetTestSeries;
			}
			//
			switch(SEGGER_RTT_GetKey())
			{
				case '1':
					if(isMainMenu)
					{
						testFunc = TestRelayTask;
						taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
					}
					else
					{
						PressPS1Button();
					}
					break;
				case '2':
					if(isMainMenu)
					{
						testFunc = TestOledTask;
						taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
					}
					else
					{
						PressPS2Button();
					}
					break;
				case '3':
					if(isMainMenu)
					{
						testFunc = TestButtonTask;
						taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
					}
					else
					{
						PressPS3Button();
					}
					break;
				case '4':
					if(isMainMenu)
					{
						testFunc = TestActivationButtonTask;
						taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
					}
					else
					{
						PressPS4Button();
					}
					break;
				case '5':
					if(isMainMenu)
					{
						testFunc = TestActivationLedTask;
						taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
					}
					else
					{
						PressPS5Button();
					}
					break;
				case '6':
					if(isMainMenu)
					{
						testFunc = TestActivationRelayTask;
						taskSeq = TestFuncTaskSeq_ExecuteTestFunc;
					}
					else
					{
						PressPS6Button();
					}
					break;
				case '7':
					if(!isMainMenu)
					{
						PressPS7Button();
					}
					break;
				case '0':
					isMainMenu = !isMainMenu;
					if(isMainMenu)
					{
						SEGGER_RTT_printf(0, "(Switch to Test Mode)\r\n\r\n");
					}
					else
					{
						SEGGER_RTT_printf(0, "(Switch to Encoder Mode)\r\n\r\n");
					}
					taskSeq = TestFuncTaskSeq_ShowHintTitle;
					break;
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
