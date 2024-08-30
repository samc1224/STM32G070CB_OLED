/*
 * I2CComm_Slave.c
 *
 *  Created on: Aug 30, 2024
 *      Author: Sam
 */
/*
#include "RTT/SEGGER_RTT.h"
#include "Comm/I2CComm_Slave.h"
#include "Generic/CRCCalc.h"
#include "main.h"

//extern I2C_HandleTypeDef hi2c2;
static uint8_t arrRx[20] = {0};
static bool isRxDone = false;
static bool isLastFrameDone = false;
static uint8_t rxIdx = 0;
static uint8_t dataLen = 0;

//ChI2C_task() use
static uint8_t errCnt = 0;
static uint8_t succCnt = 0;

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_EnableListen_IT(&hi2c2);
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
	if (I2C2 == hi2c->Instance)
	{
		if (I2C_DIRECTION_RECEIVE == TransferDirection)//master read
		{
			printf("TODO");
		}
		else if (I2C_DIRECTION_TRANSMIT == TransferDirection)//master wirte
		{
			rxIdx = 0;
			dataLen = 0;
			isLastFrameDone = false;
			//cmd header: 6 bytes
			HAL_I2C_Slave_Seq_Receive_DMA(&hi2c2, arrRx, 6, I2C_FIRST_FRAME);
			rxIdx += 6;
		}
	}
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (I2C2 == hi2c->Instance)
    {
    	if (sizeof(arrRx) > rxIdx && false == isLastFrameDone)
    	{
    		//cmd data length: header[3]
			//cmd CRC32: 4 bytes
			dataLen = arrRx[3];
			HAL_I2C_Slave_Seq_Receive_DMA(&hi2c2, arrRx + rxIdx, dataLen + 4, I2C_LAST_FRAME);
			rxIdx += (dataLen + 4);
			isLastFrameDone = true;
    	}
    	else
    	{
    		isRxDone = true;
    	}
    }
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    if (I2C2 == hi2c->Instance)
    {
        uint32_t errorCode = HAL_I2C_GetError(hi2c);

        if (errorCode & HAL_I2C_ERROR_BERR)
        {
            // Bus error
            printf("I2C Bus Error\n");
        }
        if (errorCode & HAL_I2C_ERROR_ARLO)
        {
            // Arbitration loss
            printf("I2C Arbitration Loss\n");
        }
        if (errorCode & HAL_I2C_ERROR_AF)
        {
            // Acknowledge failure
            printf("I2C Acknowledge Failure\n");
            isRxDone = true;
        }
        if (errorCode & HAL_I2C_ERROR_OVR)
        {
            // Overrun/Underrun
            printf("I2C Overrun/Underrun\n");
        }
        if (errorCode & HAL_I2C_ERROR_TIMEOUT)
        {
            // Timeout
            printf("I2C Timeout Error\n");
        }

        // Optionally: Reset the I2C peripheral
        //HAL_I2C_DeInit(hi2c);
        //HAL_I2C_Init(hi2c);
        //HAL_I2C_EnableListen_IT(&hi2c2);
    }
}

uint8_t ChI2C_getSuccCnt()
{
	return succCnt;
}

int ChI2C_clearSuccCnt()
{
	succCnt = 0;
	return 0;
}

void ChI2C_updateAddr(uint8_t addr1, uint8_t addr2)
{
	if (HAL_I2C_DeInit(&hi2c2) != HAL_OK)
	{
		Error_Handler();
	}

	//hi2c.Instance = I2C2;
	//hi2c.Init.Timing = 0x00503D58;
	hi2c2.Init.OwnAddress1 = addr1 << 1;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = addr2 << 1;
	hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if (HAL_I2C_Init(&hi2c2) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_OK != HAL_I2C_EnableListen_IT(&hi2c2))
	{
		Error_Handler();
	}

}

void ChI2C_task()
{
	if (isRxDone)
	{
		uint32_t crcResult = 0;
		isRxDone = false;

		if(!CRCCalcByValues(arrRx, CRCDataSizeType_8b, (rxIdx - 4), &crcResult, CRCDataSizeType_32b, 0xFFFFFFFF)) {
			return;
		}

		// Big Endian Series
		if (*((uint8_t *)&crcResult) == arrRx[rxIdx - 1] &&
				*((uint8_t *)&crcResult + 1) == arrRx[rxIdx - 2] &&
				*((uint8_t *)&crcResult + 2) == arrRx[rxIdx - 3] &&
				*((uint8_t *)&crcResult + 3) == arrRx[rxIdx - 4])
		{
			succCnt++;
			SEGGER_RTT_printf(0, "-> Succ Cnt:%d\r\n", succCnt);
		}
		else {
			errCnt++;
			SEGGER_RTT_printf(0, "-> Err Cnt:%d\r\n", errCnt);
		}

		memset(arrRx, 0, sizeof(arrRx));
	}
}
*/
