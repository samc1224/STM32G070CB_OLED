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
	//ChI2C_updateAddr(0x75, 0x10);//I2C2: PMH => TJB, 0x40: USU, 0x10: ETU
}

void AppTask(void)
{
	EncoderTask();
	TestFuncTask();
	//ChI2C_task();
}
