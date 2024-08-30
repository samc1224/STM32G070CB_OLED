/*
 * CRCCalc.c
 *
 *  Created on: Aug 18, 2021
 *      Author: Ching
 */

#include "Generic/CRCCalc.h"
#include "stm32g0xx_ll_crc.h"

static void ResetCRCCalc(void);

void CRCCalcInit(void)
{
	ResetCRCCalc();
}

static void ResetCRCCalc(void)
{
	LL_CRC_ResetCRCCalculationUnit(CRC);
	LL_CRC_SetInitialData(CRC, 0x00000000);
}

bool SetPolynominalParam(CRCDataSizeType crcSize, uint32_t polyValue)
{
	if(crcSize == CRCDataSizeType_8b)
	{
		LL_CRC_SetPolynomialSize(CRC, LL_CRC_POLYLENGTH_8B);
		LL_CRC_SetPolynomialCoef(CRC, (uint8_t)polyValue);
	}
	else if(crcSize == CRCDataSizeType_16b)
	{
		LL_CRC_SetPolynomialSize(CRC, LL_CRC_POLYLENGTH_16B);
		LL_CRC_SetPolynomialCoef(CRC, (uint16_t)polyValue);
	}
	else if(crcSize == CRCDataSizeType_32b)
	{
		LL_CRC_SetPolynomialSize(CRC, LL_CRC_POLYLENGTH_32B);
		LL_CRC_SetPolynomialCoef(CRC, (uint32_t)polyValue);
	}
	else
		return false;

	return true;
}

bool CRCCalcByValues(void *input, CRCDataSizeType inputSize, uint32_t inputQty, void *output, CRCDataSizeType crcSize, uint32_t xorValue)
{
	uint32_t index = 0;

	if(input == NULL || output == NULL)
		return false;

	ResetCRCCalc();

	for(index = 0; index < inputQty; index++)
	{
		if(inputSize == CRCDataSizeType_8b)
		{
			LL_CRC_FeedData8(CRC, *((uint8_t *)input + index));
		}
		else if(inputSize == CRCDataSizeType_16b)
		{
			LL_CRC_FeedData16(CRC, *((uint16_t *)input + index));
		}
		else if(inputSize == CRCDataSizeType_32b)
		{
			LL_CRC_FeedData32(CRC, *((uint32_t *)input + index));
		}
		else
			return false;
	}

	if(crcSize == CRCDataSizeType_8b)
	{
		/*~ is for Final XOR with xorValue */
		*((uint8_t *)output) = LL_CRC_ReadData8(CRC) ^ (uint8_t)xorValue;
	}
	else if(crcSize == CRCDataSizeType_16b)
	{
		/*~ is for Final XOR with xorValue */
		*((uint16_t *)output) = LL_CRC_ReadData16(CRC) ^ (uint16_t)xorValue;
	}
	else if(crcSize == CRCDataSizeType_32b)
	{
		/*~ is for Final XOR with xorValue */
		*((uint32_t *)output) = LL_CRC_ReadData32(CRC) ^ xorValue;
	}
	else
		return false;

	return true;
}


