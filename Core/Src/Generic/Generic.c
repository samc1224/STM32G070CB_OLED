/*
 * Generic.c
 *
 *  Created on: Sep 11, 2024
 *      Author: Sam
 */

#include <string.h>

#include "Comm/I2CComm_Generic.h"
#include "Generic/Generic.h"

static AppAttribute_t AppAttributeCache =
{
	.FWVersion = {0},
	.HWVersion = {0},
	.SerialNumber = {0},
	.Who = 0,
	.IsFlashWritten = 0,
};

void GenericInit(void)
{
	SetI2CCommAddress(I2CCommSlaveDriver, I2C_ADDR);

	memcpy(AppAttributeCache.FWVersion, FW_VERSION, sizeof(FW_VERSION));
	memcpy(AppAttributeCache.HWVersion, HW_VERSION, sizeof(HW_VERSION));
	memcpy(AppAttributeCache.SerialNumber, SERIAL_NUMBER, sizeof(SERIAL_NUMBER));
}

void WriteFwVersionToRsvFlash(uint8_t *servMsg)
{
	for(int i = 0; i < sizeof(AppAttributeCache.FWVersion); i++)
		AppAttributeCache.FWVersion[i] = *(servMsg + i);
}

void WriteHwVersionToRsvFlash(uint8_t *servMsg)
{
	for(int i = 0; i < sizeof(AppAttributeCache.HWVersion); i++)
		AppAttributeCache.HWVersion[i] = *(servMsg + i);
}

void WriteSerialNumberToRsvFlash(uint8_t *servMsg)
{
	for(int i = 0; i < sizeof(AppAttributeCache.SerialNumber); i++)
		AppAttributeCache.SerialNumber[i] = *(servMsg + i);
}

AppAttribute_t ReadRsvFlash(void)
{
	return AppAttributeCache;
}
