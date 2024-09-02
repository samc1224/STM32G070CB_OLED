/*
 * Encoder.h
 *
 *  Created on: Aug 29, 2024
 *      Author: Sam
 */

#ifndef INC_GENERIC_ENCODER_H_
#define INC_GENERIC_ENCODER_H_

#include <stdint.h>

void EncoderTask(void);

typedef struct
{
	uint8_t SignalRisingEXTI;
	uint8_t SignalFallingEXTI;
	uint16_t PinEXTI;
	uint16_t cntEC2;
	uint16_t cntEC1;
	uint8_t cntLED;
	uint8_t isCountUp;
	uint8_t isShowOLED;
}EncoderParam_t;

#endif /* INC_GENERIC_ENCODER_H_ */
