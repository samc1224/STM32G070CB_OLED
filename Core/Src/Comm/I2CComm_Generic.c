/*
 * I2CComm_Generic.c
 *
 *  Created on: Oct 7, 2021
 *      Author: Jackie
 *
 *  Here Shows the Protocol Model:
 *
 *  |1 Byte| Device Address
 *  |1 Byte| Protocol Version
 *  |1 Byte| Command
 *  |1 Byte| Payload Size
 *  |1 Byte| 0xAA
 *  |1 Byte| 0x55
 *  |N Byte| Payload
 *  |4 Byte| CRC
 */
#include "Comm/I2CComm_Generic.h"
#include "Comm/Postman.h"

typedef struct
{
	uint8_t DeviceType;
	uint8_t AllocateAddress;
}I2CGenericInterParam_t;

static I2CGenericInterParam_t I2CGenericInterParam =
{
	.DeviceType = THIS_WHOIS,
	.AllocateAddress = 0x7E
};

void I2CCommInit(void)
{
	/* Disable all I2C Interrupt first*/
#ifdef I2CCommMasterDriver
	LL_I2C_DisableIT_TX(I2CCommMasterDriver);
	LL_I2C_DisableIT_RX(I2CCommMasterDriver);
	LL_I2C_DisableIT_ADDR(I2CCommMasterDriver);
	LL_I2C_DisableIT_NACK(I2CCommMasterDriver);
	LL_I2C_DisableIT_STOP(I2CCommMasterDriver);
	LL_I2C_DisableIT_TC(I2CCommMasterDriver);
	LL_I2C_DisableIT_ERR(I2CCommMasterDriver);
#endif

#ifdef I2CCommSlaveDriver
	LL_I2C_DisableIT_TX(I2CCommSlaveDriver);
	LL_I2C_DisableIT_RX(I2CCommSlaveDriver);
	LL_I2C_DisableIT_ADDR(I2CCommSlaveDriver);
	LL_I2C_DisableIT_NACK(I2CCommSlaveDriver);
	LL_I2C_DisableIT_STOP(I2CCommSlaveDriver);
	LL_I2C_DisableIT_TC(I2CCommSlaveDriver);
	LL_I2C_DisableIT_ERR(I2CCommSlaveDriver);
#endif
}

void DisableI2CComm(I2C_TypeDef *I2C)
{
	LL_I2C_Disable(I2C);
}

void EnableI2CComm(I2C_TypeDef *I2C)
{
	LL_I2C_Enable(I2C);
}

void SetI2CCommAddress(I2C_TypeDef *I2C, uint8_t newAddr)
{
	LL_I2C_DisableOwnAddress1(I2C);
	LL_I2C_SetOwnAddress1(I2C, (newAddr << 1), LL_I2C_OWNADDRESS1_7BIT);
	LL_I2C_EnableOwnAddress1(I2C);
	//
	I2CGenericInterParam.AllocateAddress = newAddr;
}

void ClearI2CCommErrStatus(I2CCommTransStatus_t *I2CComm_Status)
{
	FillI2CCommErrStatus(I2CComm_Status, I2CCommError_None);
}

void FillI2CCommErrStatus(I2CCommTransStatus_t *I2CComm_Status, I2CCommError type)
{
	if(type == I2CCommError_None)
		I2CComm_Status->IsError = false;
	else
		I2CComm_Status->IsError = true;
	//
	I2CComm_Status->ErrorState = (type == I2CCommError_None) ? (1 << (uint32_t)I2CCommError_None) : (I2CComm_Status->ErrorState & ~(1 << (uint32_t)I2CCommError_None)) | (1 << (uint32_t)type);
}

uint32_t GetI2CCommErrStatus(I2CCommTransStatus_t *I2CComm_Status)
{
	return I2CComm_Status->ErrorState;
}

void ClearI2CCommPrcsStatus(I2CCommTransStatus_t *I2CComm_Status)
{
	I2CComm_Status->MatchAddr = 0;
	I2CComm_Status->IsAddrMatch = false;
	I2CComm_Status->IsSlaveWPrcs = false;
	I2CComm_Status->IsWriteFinish = false;
	I2CComm_Status->IsReadFinish = false;
	I2CComm_Status->IsWriteNACK = false;
	I2CComm_Status->WriteNACKTimeStamp = 0;
	I2CComm_Status->IsTransferBusy = false;
	I2CComm_Status->MasterTransferType = I2CCommMasterTaskType_Idle;
}

uint8_t GetCurrentSlaveAddress(void)
{
	return I2CGenericInterParam.AllocateAddress;
}
