/*
 * App.c
 *
 *  Created on: Aug 27, 2024
 *      Author: Sam
 */

#include "Comm/I2CComm_Slave.h"
#include "Comm/Postman.h"
#include "Comm/ServiceFunc.h"
#include "Display/OLEDCtrl.h"
#include "Generic/Encoder.h"
#include "Generic/Generic.h"
#include "TestFunc/TestFunc.h"

void AppInit(void)
{
	PostmanInit();
	ServFuncInit();
	GenericInit();
	OLED_Reset();
}

void AppTask(void)
{
	TestFuncTask();
	PostmanTask();
	//EncoderTask();
}
