/*
 * I2CComm_Slave.h
 *
 *  Created on: Aug 30, 2024
 *      Author: Sam
 */

#ifndef INC_COMM_I2CCOMM_SLAVE_H_
#define INC_COMM_I2CCOMM_SLAVE_H_

#include <main.h>
#include "I2CComm_Generic.h"

/**
 * @brief The Main Task of I2C Communcaiton in Slave
 *
 * @return PrcsRes_Processing The Operation is Processing
 * @return PrceRes_Failed The Operation is Failed
 * @return PrcsRes_Success The Operation is Success
 */
PrcsRes I2CCommSlaveTask(void);

/**
 * @brief Return Error Reason of Communication Slave Site
 *
 * @return uint32_t Reason of Error
 */
uint32_t GetI2CCommSlaveErrorReason(void);

/**
 * @brief Flush out the entire Buffer of I2C Communication Master Service
 *
 */
void FlushI2CCommSlaveData(void);

/**
 * @brief Setup the Transfer Task is going to Process
 *
 * @param data The Data is going to be Transfered (Only Write)
 * @param length The Length of Data
 * @return true The Operation is Valid
 * @return false The Operation is Invalid
 */
bool SetI2CCommSlaveTxData(const uint8_t *data, uint8_t length);

/**
 * @brief Get the Last Received Data Length
 *
 * @return uint8_t Length of Data has already been Received
 */
uint8_t GetI2CCommSlaveRxDataLength(void);

/**
 * @brief Get the Last Data has been Received
 *
 * @param data The Destination of Data is going to Store Last Recevied Data
 * @param length The Length of Data is going to Store
 * @return true The Operation is Valid
 * @return false The Operation is Invalid
 */
bool GetI2CCommSlaveRxData(uint8_t *data, uint8_t length);

/**
 * @brief Represent the Last I2C Communication Slave Transfer Behavior
 *
 * @return true The Last Operation is Write Procedure (S<-M)
 * @return false The Last Operation is Read Procedure (S->M)
 */
bool IsLastI2CCommSlaveRProcess(void);

void ClearLastI2CCommSlaveRProcess(void);

/**
 * @brief I2C interrupt function for Event
 *
 */
void I2CCommSlaveEVIRQHandler(void);

/**
 * @brief I2C interrupt function for Error
 *
 */
void I2CCommSlaveERIRQHandler(void);

#endif /* INC_COMM_I2CCOMM_SLAVE_H_ */
