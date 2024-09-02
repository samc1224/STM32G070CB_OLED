/*
 * TestFunc.h
 *
 *  Created on: Aug 27, 2024
 *      Author: Sam
 */

#ifndef INC_TESTFUNC_H_
#define INC_TESTFUNC_H_

#include "RTT/SEGGER_RTT.h"

typedef enum
{
	PrcsRes_Fail,
	PrcsRes_Processing,
	PrcsRes_Success
}PrcsRes;

/**
 * @brief Test Function Entry
 * 
 * @return PrcsRes Process Result
 */
PrcsRes TestFuncTask(void);

void TestOledTask(void);

void TestButtonTask(void);

void TestActivationButtonTask(void);

void TestActivationLedTask(void);

void TestActivationRelayTask(void);

#endif /* INC_TESTFUNC_H_ */
