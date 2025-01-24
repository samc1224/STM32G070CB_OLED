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

void I2CAddrInit(void)
{
	LL_I2C_DisableOwnAddress1(I2C2);
	if (SELECT_CH == 0)
		LL_I2C_SetOwnAddress1(I2C2, 0xF4, LL_I2C_OWNADDRESS1_7BIT); // CH_A: Address = 0x7A
	else
		LL_I2C_SetOwnAddress1(I2C2, 0xF6, LL_I2C_OWNADDRESS1_7BIT); // CH_B: Address = 0x7B
	LL_I2C_EnableOwnAddress1(I2C2);
}

void AppInit(void)
{
	I2CAddrInit();
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
