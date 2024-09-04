/*
 * App.c
 *
 *  Created on: Aug 27, 2024
 *      Author: Sam
 */

#include "Display/OLEDCtrl.h"
#include "Generic/Encoder.h"
#include "TestFunc/TestFunc.h"
#include "Comm/I2CComm_Slave.h"

void AppInit(void)
{
	OLED_Reset();
	I2C2_Init();
}

void AppTask(void)
{
	EncoderTask();
	TestFuncTask();
	I2C2_Listen();
}
