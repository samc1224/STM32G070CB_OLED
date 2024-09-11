/*
 * I2CComm_Slave.c
 *
 *  Created on: Aug 30, 2024
 *      Author: Sam
 */

#include "Comm/I2CComm_Slave.h"

typedef enum
{
	I2CCommTaskSeq_Conf,
	I2CCommTaskSeq_ConfAddrTask,
	I2CCommTaskSeq_WaitMatchAddress,
	I2CCommTaskSeq_SlaveWTask,
	I2CCommTaskSeq_SlaveRTask,
	I2CCommTaskSeq_ResetTask,
	I2CCommTaskSeq_ResetWait
}I2CCommSlaveTaskSeq;

static I2CCommTransStatus_t I2CCommSlaveStatus =
{
	0
};

static I2CCommBufferClct_t I2CCommSlaveBufferClct =
{
	0
};

static bool IsLastCommRPrcs_Slave = false;

void FlushI2CCommSlaveData(void)
{
	I2CCommSlaveBufferClct.BufferCursor = 0;
	memset((uint8_t *)&I2CCommSlaveBufferClct.Buffer, 0, sizeof(I2CCommSlaveBufferClct.Buffer));
}

bool SetI2CCommSlaveTxData(const uint8_t *data, uint8_t length)
{
	if(data == NULL)
		return false;
	if(length > BufSize)
		return false;
	if(I2CCommSlaveStatus.IsTransferBusy)
		return false;
	memset((uint8_t *)&I2CCommSlaveBufferClct.Buffer, 0, BufSize);
	memcpy((uint8_t *)&I2CCommSlaveBufferClct.Buffer, data, length);
	return true;
}

uint8_t GetI2CCommSlaveRxDataLength(void)
{
	return I2CCommSlaveBufferClct.BufferCursor;
}

bool GetI2CCommSlaveRxData(uint8_t *data, uint8_t length)
{
	if(data == NULL)
		return false;
	if(I2CCommSlaveStatus.IsTransferBusy)
		return false;
	memset((uint8_t *)data, 0, BufSize);
	memcpy((uint8_t *)data, (const uint8_t *)&I2CCommSlaveBufferClct.Buffer, length);
	return true;
}

bool IsLastI2CCommSlaveRProcess(void)
{
	return IsLastCommRPrcs_Slave;
}

void ClearLastI2CCommSlaveRProcess(void){
	IsLastCommRPrcs_Slave = false;
}

/*
 * Notice Current I2C Role is Slave.
 */
PrcsRes I2CCommSlaveTask(void)
{
	const uint32_t NACKTimeOut = 100; /* Represent the Timeout Target of NACK, Step is 1ms */
	const uint32_t ResetTimeOut = 1; /* Represent the Timeout Target of Reset, Step is 1ms */
	static uint32_t ResetTimeStamp; /* Represent the Timestamp of Reset Setup */

	static I2CCommSlaveTaskSeq taskSeq = I2CCommTaskSeq_Conf; /* Represent the Task Sequence */

	switch(taskSeq)
	{
		case I2CCommTaskSeq_Conf:
			/* Clear up the buffer first */
			I2CCommSlaveBufferClct.BufferCursor = 0;
			memset((uint8_t *)(&I2CCommSlaveBufferClct.Buffer), 0, sizeof(I2CCommSlaveBufferClct.Buffer) / sizeof(uint8_t));

			ClearI2CCommPrcsStatus(&I2CCommSlaveStatus);
			ClearI2CCommErrStatus(&I2CCommSlaveStatus);

			taskSeq = I2CCommTaskSeq_ConfAddrTask;
			break;
		case I2CCommTaskSeq_ConfAddrTask:
			/* Enable Address Match IRQ to receive appropriate address */
			LL_I2C_EnableIT_ADDR(I2CCommSlaveDriver);
			/* Enable the Error Interrupt */
			LL_I2C_EnableIT_ERR(I2CCommSlaveDriver);

			I2CCommSlaveStatus.IsTransferBusy = false;

			taskSeq = I2CCommTaskSeq_WaitMatchAddress;
			break;
		case I2CCommTaskSeq_WaitMatchAddress:
			if(I2CCommSlaveStatus.IsAddrMatch)
			{
				/* To make sure there is no buffer accessing during Message Transfer*/
				I2CCommSlaveStatus.IsTransferBusy = true;
				//
				if(I2CCommSlaveStatus.IsSlaveWPrcs)
				{
					taskSeq = I2CCommTaskSeq_SlaveWTask;
				}
				else
				{
					taskSeq = I2CCommTaskSeq_SlaveRTask;
				}
			}
			break;
		case I2CCommTaskSeq_SlaveWTask:
			/* If Error Occur, jump to I2C Restart Procedure */
			if(I2CCommSlaveStatus.IsError)
			{
				taskSeq = I2CCommTaskSeq_ResetTask;
				return PrcsRes_Fail;
			}

			/* If Write Procedure finish, jump to Address Detection.*/
			if(I2CCommSlaveStatus.IsWriteFinish)
			{
				IsLastCommRPrcs_Slave = false;
				/* In order to make User Insert Pack to Transfer */
				ClearI2CCommPrcsStatus(&I2CCommSlaveStatus);
				taskSeq = I2CCommTaskSeq_ConfAddrTask;
				return PrcsRes_Success;
			}

			/* If NACK occur, start to detect whether exceed the duration time*/
			if(I2CCommSlaveStatus.IsWriteNACK)
			{
				if(HAL_GetTick() - I2CCommSlaveStatus.WriteNACKTimeStamp > NACKTimeOut)
				{
					/* In order to make User Insert Pack to Transfer */
					ClearI2CCommPrcsStatus(&I2CCommSlaveStatus);
					FillI2CCommErrStatus(&I2CCommSlaveStatus, I2CCommError_NACKTimeout);
					taskSeq = I2CCommTaskSeq_ResetTask;
					return PrcsRes_Fail;
				}
			}

			taskSeq = I2CCommTaskSeq_WaitMatchAddress;
			break;
		case I2CCommTaskSeq_SlaveRTask:
			/* If Error Occur, jump to I2C Restart Procedure */
			if(I2CCommSlaveStatus.IsError)
			{
				taskSeq = I2CCommTaskSeq_ResetTask;
				return PrcsRes_Fail;
			}

			/* If Read Procedure finish, jump to Address Detection.*/
			if(I2CCommSlaveStatus.IsReadFinish)
			{
				IsLastCommRPrcs_Slave = true;
				ClearI2CCommPrcsStatus(&I2CCommSlaveStatus);
				taskSeq = I2CCommTaskSeq_ConfAddrTask;
				return PrcsRes_Success;
			}

			taskSeq = I2CCommTaskSeq_WaitMatchAddress;
			break;
		case I2CCommTaskSeq_ResetTask:
			LL_I2C_Disable(I2CCommSlaveDriver);
			if(LL_I2C_IsEnabled(I2CCommSlaveDriver) == 0)
			{
				ResetTimeStamp = HAL_GetTick();
				taskSeq = I2CCommTaskSeq_ResetWait;
			}
			break;
		case I2CCommTaskSeq_ResetWait:
			/* Shutdown the I2C for a while to make bus stable*/
			if(HAL_GetTick() - ResetTimeStamp > ResetTimeOut)
			{
				LL_I2C_Enable(I2CCommSlaveDriver);
				ClearI2CCommPrcsStatus(&I2CCommSlaveStatus);
				ClearI2CCommErrStatus(&I2CCommSlaveStatus);
				taskSeq = I2CCommTaskSeq_ConfAddrTask;
			}
			break;
	}

	return PrcsRes_Processing;
}

uint32_t GetI2CCommSlaveErrorReason(void)
{
	return GetI2CCommErrStatus(&I2CCommSlaveStatus);
}

void I2CCommSlaveEVIRQHandler(void)
{
	/* If the calling address matches our address or ReStart Generated*/
	if(LL_I2C_IsActiveFlag_ADDR(I2CCommSlaveDriver) == 1 && LL_I2C_IsEnabledIT_ADDR(I2CCommSlaveDriver) == 1)
	{
		LL_I2C_ClearFlag_ADDR(I2CCommSlaveDriver);
		I2CCommSlaveStatus.IsAddrMatch = true;
		I2CCommSlaveStatus.IsWriteNACK = false;

		I2CCommSlaveBufferClct.BufferCursor = 0;

		/* Write Transfer means Master Write, Slave Read*/
		if(LL_I2C_GetTransferDirection(I2CCommSlaveDriver) == LL_I2C_DIRECTION_WRITE)
		{
			LL_I2C_DisableIT_TX(I2CCommSlaveDriver);
			LL_I2C_EnableIT_RX(I2CCommSlaveDriver);
			LL_I2C_DisableIT_NACK(I2CCommSlaveDriver);
			LL_I2C_EnableIT_STOP(I2CCommSlaveDriver);

			I2CCommSlaveStatus.IsSlaveWPrcs = false;
		}
		/* Read Transfer means Master Read, Slave Write*/
		else
		{
			LL_I2C_EnableIT_TX(I2CCommSlaveDriver);
			LL_I2C_DisableIT_RX(I2CCommSlaveDriver);
			LL_I2C_EnableIT_NACK(I2CCommSlaveDriver);
			LL_I2C_EnableIT_STOP(I2CCommSlaveDriver);

			I2CCommSlaveStatus.IsSlaveWPrcs = true;
			LL_I2C_TransmitData8(I2CCommSlaveDriver, I2CCommSlaveBufferClct.Buffer[I2CCommSlaveBufferClct.BufferCursor++]);
		}

		I2CCommSlaveStatus.MatchAddr = (uint16_t)LL_I2C_GetAddressMatchCode(I2CCommSlaveDriver);
	}

	if(I2CCommSlaveStatus.IsSlaveWPrcs)
	{
		/* STOP Procedure */
		if(LL_I2C_IsActiveFlag_STOP(I2CCommSlaveDriver) == 1 && LL_I2C_IsEnabledIT_STOP(I2CCommSlaveDriver) == 1)
		{
			/* Flush out the shadow register */
			LL_I2C_ClearFlag_TXE(I2CCommSlaveDriver);
			LL_I2C_ClearFlag_STOP(I2CCommSlaveDriver);
			//
			I2CCommSlaveStatus.IsWriteNACK = false;
			I2CCommSlaveStatus.IsWriteFinish = true;
			return;
		}

		/* NACK Procedure */
		if(LL_I2C_IsActiveFlag_NACK(I2CCommSlaveDriver) == 1 && LL_I2C_IsEnabledIT_NACK(I2CCommSlaveDriver) == 1)
		{
			/* Flush out the shadow register */
			LL_I2C_ClearFlag_TXE(I2CCommSlaveDriver);
			LL_I2C_ClearFlag_NACK(I2CCommSlaveDriver);

			I2CCommSlaveStatus.IsWriteNACK = true;
			I2CCommSlaveStatus.WriteNACKTimeStamp = HAL_GetTick();
			return;
		}

		if(LL_I2C_IsActiveFlag_TXE(I2CCommSlaveDriver) == 1 && LL_I2C_IsActiveFlag_TXIS(I2CCommSlaveDriver) == 1 && LL_I2C_IsEnabledIT_TX(I2CCommSlaveDriver) == 1)
		{
			LL_I2C_TransmitData8(I2CCommSlaveDriver, I2CCommSlaveBufferClct.Buffer[I2CCommSlaveBufferClct.BufferCursor++]);
		}
	}
	else
	{
		/* STOP Procedure */
		if(LL_I2C_IsActiveFlag_STOP(I2CCommSlaveDriver) == 1 && LL_I2C_IsEnabledIT_STOP(I2CCommSlaveDriver) == 1)
		{
			LL_I2C_ClearFlag_STOP(I2CCommSlaveDriver);

			I2CCommSlaveStatus.IsReadFinish = true;
			return;
		}

		if(LL_I2C_IsActiveFlag_RXNE(I2CCommSlaveDriver) == 1 && LL_I2C_IsEnabledIT_RX(I2CCommSlaveDriver) == 1)
		{
			/* Because the speed is fast, so data accessing can not be implemented in regular procedure.*/
			I2CCommSlaveBufferClct.Buffer[I2CCommSlaveBufferClct.BufferCursor++] = LL_I2C_ReceiveData8(I2CCommSlaveDriver);
		}
	}
}

void I2CCommSlaveERIRQHandler(void)
{
	/*
	 * SMBus function only I2C1 can use, I2C2 is not support. Reference manual (25.3 I2C implementation)
	 * */
#if 0
	if(LL_I2C_IsActiveSMBusFlag_TIMEOUT(I2CCommSlaveDriver) > 0)
	{
		LL_I2C_ClearSMBusFlag_TIMEOUT(I2CCommSlaveDriver);
		FillI2CCommErrStatus(&I2CCommSlaveStatus, I2CCommError_TimeoutError);
	}

	if(LL_I2C_IsActiveSMBusFlag_PECERR(I2CCommSlaveDriver) > 0)
	{
		LL_I2C_ClearSMBusFlag_PECERR(I2CCommSlaveDriver);
		FillI2CCommErrStatus(&I2CCommSlaveStatus, I2CCommError_PECError);
	}
#endif

	if(LL_I2C_IsActiveFlag_BERR(I2CCommSlaveDriver) > 0)
	{
		LL_I2C_ClearFlag_BERR(I2CCommSlaveDriver);
		FillI2CCommErrStatus(&I2CCommSlaveStatus, I2CCommError_BusError);
	}

	if(LL_I2C_IsActiveFlag_OVR(I2CCommSlaveDriver) > 0)
	{
		LL_I2C_ClearFlag_OVR(I2CCommSlaveDriver);
		FillI2CCommErrStatus(&I2CCommSlaveStatus, I2CCommError_OverRun);
	}

	if(LL_I2C_IsActiveFlag_ARLO(I2CCommSlaveDriver) > 0)
	{
		LL_I2C_ClearFlag_ARLO(I2CCommSlaveDriver);
		FillI2CCommErrStatus(&I2CCommSlaveStatus, I2CCommError_ArbError);
	}
}
