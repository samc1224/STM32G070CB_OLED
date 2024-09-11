/*
 * Generic.h
 *
 *  Created on: Sep 11, 2024
 *      Author: Sam
 */

#ifndef INC_GENERIC_GENERIC_H_
#define INC_GENERIC_GENERIC_H_

#include <stdint.h>

/* FW version shall read only */
#define FW_VERSION "010"
#define HW_VERSION "100"
#define SERIAL_NUMBER "Ab99999999"

#define Is(x) (x == 1)
#define Not(x) (x == 0)

#define THIS_WHOIS	(0x07)

typedef enum
{
	PrcsRes_Fail,
	PrcsRes_Processing,
	PrcsRes_Success
}PrcsRes;

typedef struct __attribute__((aligned(8)))
{
	uint8_t FWVersion[3];
	uint8_t HWVersion[3];
	uint8_t SerialNumber[10];
	uint8_t Who;
	uint8_t IsFlashWritten;
}AppAttribute_t;

/**
 * @brief Generic Part Initialization
 *
 */
void GenericInit(void);

/**
 * @brief Write FW Version To Rsv Flash
 *
 */
void WriteFwVersionToRsvFlash(uint8_t *servMsg);

/**
 * @brief Write HW Version To Rsv Flash
 *
 */
void WriteHwVersionToRsvFlash(uint8_t *servMsg);

/**
 * @brief Write Serial Number To Rsv Flash
 *
 */
void WriteSerialNumberToRsvFlash(uint8_t *servMsg);

/**
 * @brief Read AppAttributeCache from Rsv Flash
 *
 */
AppAttribute_t ReadRsvFlash(void);

#endif /* INC_GENERIC_GENERIC_H_ */
