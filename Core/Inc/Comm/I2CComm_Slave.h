/*
 * I2CComm_Slave.h
 *
 *  Created on: Aug 30, 2024
 *      Author: Sam
 */

#ifndef INC_COMM_I2CCOMM_SLAVE_H_
#define INC_COMM_I2CCOMM_SLAVE_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

void ChI2C_updateAddr(uint8_t addr1, uint8_t addr2);
void ChI2C_task();

uint8_t ChI2C_getSuccCnt();
int ChI2C_clearSuccCnt();


#endif /* INC_COMM_I2CCOMM_SLAVE_H_ */
