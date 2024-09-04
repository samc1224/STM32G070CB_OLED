/*
 * Encoder.h
 *
 *  Created on: Aug 29, 2024
 *      Author: Sam
 */

#ifndef INC_GENERIC_ENCODER_H_
#define INC_GENERIC_ENCODER_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct
{
	bool EXTI_RisingEdge;
	bool EXTI_FallingEdge;
	bool isShowCount;
	bool isCountUp;
	uint16_t cntEC1;
	uint16_t cntEC2;
	uint8_t cntIndex;
	uint16_t cntRelay;
}EncoderParam_t;

EncoderParam_t ReadEncoderParam(void);

void WriteEncoderParam(EncoderParam_t param);

void SetEncoderShowCount(bool setShow);

void EncoderInit(void);

void EncoderTask(void);

#endif /* INC_GENERIC_ENCODER_H_ */
