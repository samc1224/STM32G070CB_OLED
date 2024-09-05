/*
 * I2CComm_Slave.c
 *
 *  Created on: Aug 30, 2024
 *      Author: Sam
 */

#include "main.h"
#include "Comm/I2CComm_Slave.h"

#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Size of Transmission buffer */
#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer))
/* Size of Reception buffer */
#define RXBUFFERSIZE                      TXBUFFERSIZE

__IO uint32_t     Transfer_Direction = 0;
__IO uint32_t     Xfer_Complete = 0;

/* Buffer used for transmission */
uint8_t aTxBuffer[4];
/* Buffer used for reception */
uint8_t aRxBuffer[4];

/*
void I2C2_Reset()
{
    // Open drain output, close the I2C input channel and try to pull the bus high
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10 | GPIO_PIN_11, GPIO_PIN_SET);

    // SCL PB8 pull high
    for (uint8_t i = 0; i < 10; i++) {
        if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_SET)
        {
            break;
        }
        // The period and duration of this delay loop should be modified based on how your actual host handles I2C communication errors.
        HAL_Delay(10);
    }

    // Return bus control
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Reset I2C
    hi2c2.Instance->CR1 |= I2C_CR1_SWRST;
    hi2c2.Instance->CR1 &= ~I2C_CR1_SWRST;

    // Reinitialize I2C
    MX_I2C2_Init();
}
*/

void I2C2_Init()
{
	aRxBuffer[0]=0x00;
	aRxBuffer[1]=0x00;
	aRxBuffer[2]=0x00;
	aRxBuffer[3]=0x00;
	aTxBuffer[0]=0xAA;
	aTxBuffer[1]=0xBB;
	aTxBuffer[2]=0xCC;
	aTxBuffer[3]=0xDD;

	/* Put I2C peripheral in listen mode process */
	if(HAL_I2C_EnableListen_IT(&hi2c2) != HAL_OK)
	{
		Error_Handler();
	}

	/*
	if (HAL_I2C_Slave_Receive_DMA(&hi2c2, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
	{
		Error_Handler();
	}

	//while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
	{
	}

	//if (HAL_I2C_Slave_Transmit_DMA(&hi2c2, (uint8_t *)aTxBuffer, TXBUFFERSIZE) != HAL_OK)
	{
		//Error_Handler();
	}
	*/
}

void I2C2_Listen()
{
	if (Xfer_Complete == 1)
	{
		Xfer_Complete = 0;
		HAL_I2C_Init(&hi2c2);
		/* Put I2C peripheral in listen mode process */
		if(HAL_I2C_EnableListen_IT(&hi2c2) != HAL_OK)
		{
			Error_Handler();
		}
		/*
		if (HAL_I2C_Slave_Transmit_DMA(&hi2c2, (uint8_t *)aTxBuffer, TXBUFFERSIZE) != HAL_OK)
		{
			Error_Handler();
		}
		*/
	}
}

/**
  * @brief  Tx Transfer completed callback.
  *   I2cHandle: I2C handle.
  * @note   This example shows a simple way to report end of IT Tx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
	/*
	if (HAL_I2C_Slave_Receive_DMA(&hi2c2, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
	{
		Error_Handler();
	}
	*/

  Xfer_Complete = 1;
  aTxBuffer[0]++;
  aTxBuffer[1]++;
  aTxBuffer[2]++;
  aTxBuffer[3]++;
}

/**
  * @brief  Rx Transfer completed callback.
  *   I2cHandle: I2C handle
  * @note   This example shows a simple way to report end of IT Rx transfer, and you can add your own implementation.
  * @retval None
  */
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  Xfer_Complete = 1;
  aRxBuffer[0]=0x00;
  aRxBuffer[1]=0x00;
  aRxBuffer[2]=0x00;
  aRxBuffer[3]=0x00;
}

/**
  * @brief  Slave Address Match callback.
  *   hi2c Pointer to a I2C_HandleTypeDef structure that contains the configuration information for the specified I2C.
  *   TransferDirection: Master request Transfer Direction (Write/Read), value of @ref I2C_XferOptions_definition
  *   AddrMatchCode: Address Match Code
  * @retval None
  */
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
  Transfer_Direction = TransferDirection;
  if (Transfer_Direction != 0)
  {
	/*##- Start the transmission process #####################################*/
	/* While the I2C in reception process, user can transmit data through "aTxBuffer" buffer */
	if (HAL_I2C_Slave_Seq_Transmit_IT(&hi2c2, (uint8_t *)aTxBuffer, TXBUFFERSIZE, I2C_FIRST_AND_LAST_FRAME) != HAL_OK)
	{
		/* Transfer error in transmission process */
		Error_Handler();
	}
  }
  else
  {
	/*##- Put I2C peripheral in reception process ###########################*/
	if (HAL_I2C_Slave_Seq_Receive_IT(&hi2c2, (uint8_t *)aRxBuffer, RXBUFFERSIZE, I2C_FIRST_AND_LAST_FRAME) != HAL_OK)
	{
		/* Transfer error in reception process */
		Error_Handler();
	}
  }
}

/**
  * @brief  Listen Complete callback.
  *   hi2c Pointer to a I2C_HandleTypeDef structure that contains the configuration information for the specified I2C.
  * @retval None
  */
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
}

/**
  * @brief  I2C error callbacks.
  *   I2cHandle: I2C handle
  * @note   This example shows a simple way to report transfer error, and you can add your own implementation.
  * @retval None
  */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
  /** Error_Handler() function is called when error occurs.
    * 1- When Slave doesn't acknowledge its address, Master restarts communication.
    * 2- When Master doesn't acknowledge the last data transferred, Slave doesn't care in this example.
    */
  if (HAL_I2C_GetError(I2cHandle) != HAL_I2C_ERROR_AF)
  {
    Error_Handler();
  }
}
