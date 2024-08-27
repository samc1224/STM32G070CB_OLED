/*
 * TestFunc.c
 *
 *  Created on: Aug 27, 2024
 *      Author: Sam
 */

#include <stdbool.h>
#include "main.h"
#include "TestFunc/TestFunc.h"

#include "Display/OLEDCtrl.h"

typedef enum
{
	TestFuncTaskSeq_ShowHintTitle,
	TestFuncTaskSeq_GetTestSeries,
	TestFuncTaskSeq_ExecuteTestFunc,
	TestFuncTaskSeq_Exit
}TestFuncTaskSeq;

/* Entry of Test Function */
typedef void (* TestFunctionEntry)(void);

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
			SEGGER_RTT_printf(0, "-> Click '1' > OLED Test\r\n");
			SEGGER_RTT_printf(0, "-> Click '2' > Button & LED Test\r\n");
			OLED_ShowString_11x18("Main Menu");
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
