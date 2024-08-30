/*
 * CRCCalc.h
 *
 *  Created on: Aug 18, 2021
 *      Author: Ching
 */

#ifndef INC_GENERIC_CRCCALC_H_
#define INC_GENERIC_CRCCALC_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief The Size of CRC Data
 * 
 */
typedef enum
{
	CRCDataSizeType_8b,
	CRCDataSizeType_16b,
	CRCDataSizeType_32b
}CRCDataSizeType;

/**
 * @brief CRC Calculation Service Initialize
 * 
 */
void CRCCalcInit(void);

/**
 * @brief Set the Polynominal Param of CRC
 * 
 * @param polySize The Size of CRC
 * @param polyValue The Polynominal Value of CRC
 * @return true Operation is Valid
 * @return false Operation is Invalid
 */
bool SetPolynominalParam(CRCDataSizeType polySize, uint32_t polyValue);

/**
 * @brief Calculate the CRC Value by input Series
 * 
 * @param input The Input Data
 * @param inputSize The Size of Input Data
 * @param inputQty The Quantity of Input Data
 * @param output The Output Data
 * @param outputSize The Size of Output Data
 * @param xorValue The Final XOR Value is going to be Operated
 * @return true Operation is Valid
 * @return false Operation is Invalid
 */
bool CRCCalcByValues(void *input, CRCDataSizeType inputSize, uint32_t inputQty, void *output, CRCDataSizeType outputSize, uint32_t xorValue);

#endif /* INC_GENERIC_CRCCALC_H_ */
