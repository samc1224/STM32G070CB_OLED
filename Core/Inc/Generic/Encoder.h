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
	bool isCountUp;
	bool isResistorConv;
	uint16_t cntEC1;
	uint16_t cntEC2;
	uint8_t cntIndex;
	uint16_t cntRawValue;
	uint16_t cntRawValueTmp;
	uint8_t cntMultipleBig;
	uint8_t cntMultipleSmall;
}EncoderParam_t;

void ChangeEncoderBigMultiple();
void ChangeEncoderSmallMultiple();
EncoderParam_t ReadEncoderParam(void);
void WriteEncoderParam(EncoderParam_t param);
void SetEncoderResistorConversion(bool isConv);
void SetEncoderCountRawValue(uint16_t cnt);
void EncoderInit(void);
void EncoderTask(void);

#endif /* INC_GENERIC_ENCODER_H_ */
