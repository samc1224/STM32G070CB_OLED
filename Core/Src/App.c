/*
 * App.c
 *
 *  Created on: Aug 27, 2024
 *      Author: Sam
 */

#include "Display/OLEDCtrl.h"
#include "Generic/Encoder.h"
#include "TestFunc/TestFunc.h"

void AppInit(void)
{
	OLED_Reset();
}

void AppTask(void)
{
	EncoderTask();
	TestFuncTask();
}
