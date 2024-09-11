/*
 * Postman.c
 *
 *  Created on: Aug 19, 2021
 *      Author: ching
 */

#include <stdlib.h>
#include <string.h>

#include "Comm/Postman.h"

#include "Comm/I2CComm_Slave.h"
#include "Comm/I2CComm_Generic.h"

#include "Generic/CRCCalc.h"

typedef enum
{
	PostmanTaskSeq_Conf,
	PostmanTaskSeq_TransferDuty,
	PostmanTaskSeq_ProcessMessage,
	PostmanTaskSeq_Post
}PostmanTaskSeq;

static PostArvCallback PostArvEntryList[MaxSizePostArvEntryList];

static uint32_t PostmanErrorState;

static void RecordPostmanErrState(PostmanErrorType type);

void PostmanInit(void)
{
	memset(PostArvEntryList, 0, sizeof(PostArvEntryList));
	//
	I2CCommInit();
	CRCCalcInit();
}

bool RegisterPostArvEntry(uint8_t adrKey, PostArvCallback entry)
{
	/* If current entry is empty, fill in and finish the registration*/
	if(PostArvEntryList[adrKey] == NULL)
	{
		PostArvEntryList[adrKey] = entry;
		return true;
	}
	//
	RecordPostmanErrState(PostmanErrorType_Registration);
	return false;
}

bool UnregisterPostArvEntry(uint8_t adrKey, PostArvCallback entry)
{
	/* Before Unregister the Entry, Check whether specific entry is the same as release one.*/
	if(PostArvEntryList[adrKey] == entry)
	{
		PostArvEntryList[adrKey] = NULL;
		return true;
	}
	//
	RecordPostmanErrState(PostmanErrorType_UnRegistration);
	return false;
}

bool ReplyPostToSender(uint8_t command, uint8_t *msg, uint8_t size)
{
	const uint8_t SizeOfPrefix = 6; /* Represent the Size of Prefix in Pack */
	const uint8_t SizeOfCRC = 4; /* Represent the Size of CRC in Pack */

	uint32_t genCRC32 = 0; /* Represent the CRC Generation Result */
	uint8_t asmbPack[MaxSizePostMessage]; /* Represent the Assembled Pack */

	if(msg == NULL || (SizeOfPrefix + size + SizeOfCRC) > MaxSizePostMessage)
	{
		RecordPostmanErrState(PostmanErrorType_ReplyPost);
		return false;
	}

	/* Reserved (Fixed) Prefix in Pack */
	asmbPack[0] = GetCurrentSlaveAddress(); /* Need to be filled with Slave Address, need involve with I2C AllocaAdr Service */
	asmbPack[1] = ProtocolVersion;
	asmbPack[2] = command;
	asmbPack[3] = size;
	asmbPack[4] = 0x55;
	asmbPack[5] = 0xaa;

	/* Copy the sending data to Assemble Package */
	memcpy(&asmbPack[6], msg, size * sizeof(uint8_t));

	/* Generate the CRC */
	if(!CRCCalcByValues(asmbPack, CRCDataSizeType_8b, (SizeOfPrefix + size), &genCRC32, CRCDataSizeType_32b, 0xFFFFFFFF))
	{
		RecordPostmanErrState(PostmanErrorType_CRC);
		return false;
	}

	/* Fill in the CRC into Assemble Package */
	asmbPack[(SizeOfPrefix + size)] = *((uint8_t *)&genCRC32 + 3);
	asmbPack[((SizeOfPrefix + size) + 1)] = *((uint8_t *)&genCRC32 + 2);
	asmbPack[((SizeOfPrefix + size) + 2)] = *((uint8_t *)&genCRC32 + 1);
	asmbPack[((SizeOfPrefix + size) + 3)] = *((uint8_t *)&genCRC32);

	/* Add extra 4 bytes for CRC storage */
	if(!SetI2CCommSlaveTxData(asmbPack, (SizeOfPrefix + size + SizeOfCRC)))
	{
		/* In this Situation, maybe the Communication Driver is Busy */
		return false;
	}
	//
	return true;
}

PrcsRes PostmanTask(void)
{
	const uint8_t MinimumPackageSize = 10; /* Represent the Minimum Size of Pack Received */

	static PostmanTaskSeq taskSeq = PostmanTaskSeq_Conf; /* Represent the Task Sequence */

	static I2CCommFlushTransferBuffer flushFunc = NULL; /* Represent the Function Entry of Buffer Flush */
	static I2CCommGetReceivedLength getReceivedLengthFunc = NULL; /* Represent the Function Entry of getting Received Length */
	static I2CCommGetReceivedData getReceivedDataFunc = NULL; /* Represent the Function Entry of Receive Data */

	uint8_t postMessage[MaxSizePostMessage]; /* Represent the Message is going to be Processed */
	uint8_t postMsgSize = 0; /* Represent the Size of Post Message */
	uint32_t crcResult = 0; /* Represent the CRC Calculation Result */

	PrcsRes commTaskRes = PrcsRes_Processing; /* Represent the Result of CommTask */

	switch(taskSeq)
	{
		case PostmanTaskSeq_Conf:
			RecordPostmanErrState(PostmanErrorType_None);
			taskSeq = PostmanTaskSeq_TransferDuty;
			break;
		case PostmanTaskSeq_TransferDuty:
			commTaskRes = I2CCommSlaveTask();
			/* if Comm Task Result is failed, Record the Fail Reason */
			if(commTaskRes == PrcsRes_Fail)
			{
				RecordPostmanErrState(PostmanErrorType_CommFail);
			}
			/* If in Slave Task, Last Transfer Command is R Process (Slave <- Master) Process the return Message */
			else if(commTaskRes == PrcsRes_Success && IsLastI2CCommSlaveRProcess())
			{
				flushFunc = FlushI2CCommSlaveData;
				getReceivedLengthFunc = GetI2CCommSlaveRxDataLength;
				getReceivedDataFunc = GetI2CCommSlaveRxData;
				taskSeq = PostmanTaskSeq_ProcessMessage;
				break;
			}

			break;
		case PostmanTaskSeq_ProcessMessage:
			/* If one of the Function Entry is NULL, the Procedure is Abnormal, Skip this turn of Processing */
			if(flushFunc == NULL || getReceivedLengthFunc == NULL || getReceivedDataFunc == NULL)
			{
				taskSeq = PostmanTaskSeq_TransferDuty;
				return PrcsRes_Fail;
			}
			//
			postMsgSize = getReceivedLengthFunc();
			/* Get the data from I2C Communication Buffer*/
			if(!getReceivedDataFunc(postMessage, postMsgSize))
			{
				RecordPostmanErrState(PostmanErrorType_RecvPost);
				taskSeq = PostmanTaskSeq_TransferDuty;
				return PrcsRes_Fail;
			}
			/*	|1 Byte| Device Address
			 *  |1 Byte| Protocol Version
			 *  |1 Byte| Command
			 *  |1 Byte| Payload Size
			 *  |1 Byte| 0xAA
			 *  |1 Byte| 0x55
			 *  |N Byte| Payload
			 *  |4 Byte| CRC */
			if(postMsgSize < MinimumPackageSize)
			{
				flushFunc();
				RecordPostmanErrState(PostmanErrorType_PostSize);
				taskSeq = PostmanTaskSeq_TransferDuty;
				return PrcsRes_Fail;
			}
			/* Calculate the CRC, and check correction */
			if(!CRCCalcByValues(postMessage, CRCDataSizeType_8b, (postMsgSize - 4), &crcResult, CRCDataSizeType_32b, 0xFFFFFFFF))
			{
				flushFunc();
				RecordPostmanErrState(PostmanErrorType_CRC);
				taskSeq = PostmanTaskSeq_TransferDuty;
				return PrcsRes_Fail;
			}
			//
			/* Big Endian Series */
			if(!(*((uint8_t *)&crcResult) == postMessage[(postMsgSize - 1)] &&
					*((uint8_t *)&crcResult + 1) == postMessage[(postMsgSize - 2)] &&
					*((uint8_t *)&crcResult + 2) == postMessage[(postMsgSize - 3)] &&
					*((uint8_t *)&crcResult + 3) == postMessage[(postMsgSize - 4)]))
			{
				flushFunc();
				RecordPostmanErrState(PostmanErrorType_CRC);
				taskSeq = PostmanTaskSeq_TransferDuty;
				return PrcsRes_Fail;
			}
			/* Receiver Symbol*/
			uint8_t receiverSymbol = postMessage[2];
			/* If no receiver entry info exists in list, discard this message*/
			if(PostArvEntryList[receiverSymbol] == NULL)
			{
				flushFunc();
				RecordPostmanErrState(PostmanErrorType_NoReceiverExist);
				taskSeq = PostmanTaskSeq_TransferDuty;
				return PrcsRes_Fail;
			}
			/* Send the message to specific receiver */
			PostArvEntryList[receiverSymbol](postMessage[2], &postMessage[6], postMessage[3]);
			//
			taskSeq = PostmanTaskSeq_TransferDuty;
			return PrcsRes_Success;
		case PostmanTaskSeq_Post:
			/* No Trap at here, Postman DO NOT Need to take a break.*/
			taskSeq = PostmanTaskSeq_TransferDuty;
			break;
	}
	//
	return PrcsRes_Processing;
}

static void RecordPostmanErrState(PostmanErrorType type)
{
	PostmanErrorState = (type == PostmanErrorType_None) ? (1 << PostmanErrorType_None) : (PostmanErrorState & ~(1 << PostmanErrorType_None)) | (1 << type);
}

uint32_t GetPostmanErrStatus(void)
{
	uint32_t ret = PostmanErrorState; /* Represent the Return Reasons of Error */
	RecordPostmanErrState(PostmanErrorType_None);
	return ret;
}
