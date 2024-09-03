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
	//ChI2C_updateAddr(0x75, 0x10);//I2C2: PMH => TJB, 0x40: USU, 0x10: ETU
}

void AppTask(void)
{
	EncoderTask();
	TestFuncTask();
	I2C2_Listen();
	//ChI2C_task();
}
