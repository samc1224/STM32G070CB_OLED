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
	uint8_t cntBigMultiple;
	uint8_t cntSmallMultiple;
}EncoderParam_t;

void ShowEncoderCount(void);
void RawValueBigChange(bool isCntUp);
void RawValueSmallChange(bool isCntUp);

void ChangeEncoderBigMultiple(uint8_t cntSmall);
void ChangeEncoderSmallMultiple(uint8_t cntSmall);
EncoderParam_t ReadEncoderParam(void);
void WriteEncoderParam(EncoderParam_t param);
void SetEncoderResistorConversion(bool isConv);
void SetEncoderCountRawValue(uint16_t cnt);
void EncoderInit(void);
void EncoderTask(void);

#endif /* INC_GENERIC_ENCODER_H_ */
