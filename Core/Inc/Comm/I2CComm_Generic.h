/*
 * I2CComm_Generic.h
 *
 *  Created on: Oct 7, 2021
 *      Author: Jackie
 */

#ifndef INC_COMM_I2CCOMM_GENERIC_H_
#define INC_COMM_I2CCOMM_GENERIC_H_

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"

#include "Generic/Generic.h"

/**
 * @brief Depend on Different Device, Change the Header File
 * 
 */
#include "stm32g070xx.h"

/**
 * @brief The Responsible Driver for I2C Communication Master
 * 
 */
//#define I2CCommMasterDriver	I2C2

/**
 * @brief The Responsible Driver for I2C Communication Slave
 * 
 */
#define I2CCommSlaveDriver	I2C2

/**
 * @brief The Maximum Size of I2C Communication Service is able to Transfer
 * 
 */
#define BufSize	150 // 0~254

/**
 * @brief The Behavior of Master is going to Process
 * 
 */
typedef enum
{
	I2CCommMasterTaskType_W, /* Write Process (Master -> Slave) */
	I2CCommMasterTaskType_R, /* Read Process (Master <- Slave) */
	I2CCommMasterTaskType_Idle /* Idle (No Transfer is going to Process) */
}I2CCommMasterTaskType;

/**
 * @brief The Possible Error Status will Occur during Communication Task
 * 
 */
typedef enum
{
	I2CCommError_None = 0,
	I2CCommError_TimeoutError,
	I2CCommError_PECError,
	I2CCommError_OverRun,
	I2CCommError_BusError,
	I2CCommError_ArbError,
	I2CCommError_NACKTimeout,
	I2CCommError_UnconditionalSituation,
	I2CCommError_DMAError,
}I2CCommError;

/**
 * @brief The Communication Task Process Status
 * 
 */
typedef struct
{
	volatile uint16_t MatchAddr; /* Represent The Matched Address in Slave Mode */
	bool IsTransferBusy; /* Represent whether Current Communication Task is Processing */
	volatile bool IsAddrMatch; /* Represent whether Any Address Occured in Slave Mode */
	volatile bool IsSlaveWPrcs; /* Represent Current Access Behavior of Salve Mode */
	volatile bool IsMasterWPrcs; /* Represent Current Access Behavior of Master Mode */
	volatile bool IsWriteFinish; /* Represent whether Current Write Procedure (M->S or S->M) has Finished */
	volatile bool IsWriteNACK; /* Represent whether NACK Occurred during Transfer */
	volatile bool IsReadFinish; /* Represent whether Current Read Procedure (M<-S ot S<-M) has Finished */
	volatile bool IsError; /* Represent whether any Error Occurred during the Task Procedure */
	I2CCommMasterTaskType MasterTransferType; /* Represent the Behavior of Master is going to Processing */
	uint32_t ErrorState; /* Represent Error Reason during the Procedure */
	volatile uint32_t WriteNACKTimeStamp; /* Represent the Timestmp of NACK Occurred */
}I2CCommTransStatus_t;

/**
 * @brief The Communication Buffer Collection
 * 
 */
typedef struct
{
	uint8_t Address; /* Represent the Address is going to send, Ignore this in Slave Mode */
	volatile uint8_t Buffer[BufSize]; /* The Transfer Buffer */
	volatile uint16_t BufferCursor; /* The Size of Buffer is going to Process */
}I2CCommBufferClct_t;

/**
 * @brief Function Pointer of Flush Buffer
 * 
 */
typedef void (*I2CCommFlushTransferBuffer)(void);

/**
 * @brief Function Pointer of Getting Received Data Size
 * 
 */
typedef uint8_t (*I2CCommGetReceivedLength)(void);

/**
 * @brief Function Pointer of Getting Received Data
 * 
 */
typedef bool (*I2CCommGetReceivedData)(uint8_t *data, uint8_t length);

/**
 * @brief I2C Communication Service Initialize
 * 
 */
void I2CCommInit(void);

/**
 * @brief Disable I2C Communication Function
 * 
 * @param I2C Driver to be Disabled
 */
void DisableI2CComm(I2C_TypeDef *I2C);

/**
 * @brief Enable I2C Communication Function
 * 
 * @param I2C Driver to be Enabled
 */
void EnableI2CComm(I2C_TypeDef *I2C);

/**
 * @brief Set I2C Slave Address
 * 
 * @param I2C The Interface of Driver
 * @param newAddr The Address is going to Set
 */
void SetI2CCommAddress(I2C_TypeDef *I2C, uint8_t newAddr);

/**
 * @brief Reset I2C Communication Service Process Status
 * 
 * @param I2CComm_Status The Status is going to be Clear
 */
void ClearI2CCommPrcsStatus(I2CCommTransStatus_t *I2CComm_Status);

/**
 * @brief Clearr I2C Communication Service Error Status
 * 
 * @param I2CComm_Status The Status is goint to be Clear
 */
void ClearI2CCommErrStatus(I2CCommTransStatus_t *I2CComm_Status);

/**
 * @brief Set I2C Communication Service Error Status
 * 
 * @param I2CComm_Status The Status is going to be recorded
 * @param type The Error Type is going to be filled
 */
void FillI2CCommErrStatus(I2CCommTransStatus_t *I2CComm_Status, I2CCommError type);

/**
 * @brief Get I2C Communication Service Error Status
 * 
 * @param I2CComm_Status The Status is going to be fetched
 * @return uint32_t The Error Type has been filled in Status
 */
uint32_t GetI2CCommErrStatus(I2CCommTransStatus_t *I2CComm_Status);

/**
 * @brief Get current I2C Address
 *
 * @return uint8_t Current I2C Address in Slave side
 */
uint8_t GetCurrentSlaveAddress(void);

#endif /* INC_COMM_I2CCOMM_GENERIC_H_ */
