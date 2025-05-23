/*
 * App.c
 *
 *  Created on: Aug 27, 2024
 *      Author: Sam
 */

#include "Comm/Postman.h"
#include "Comm/ServiceFunc.h"
#include "Display/OLEDCtrl.h"
#include "Generic/Encoder.h"
#include "Generic/Generic.h"
#include "TestFunc/TestFunc.h"

void AppInit(void)
{
	GenericInit();
	PostmanInit();
	ServFuncInit();
	OLED_Reset();
}

void AppTask(void)
{
	TestFuncTask();
	PostmanTask();
	//EncoderTask();
}
