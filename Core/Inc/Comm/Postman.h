/*
 * Postman.h
 *
 *  Created on: Aug 19, 2021
 *      Author: ching
 */

#ifndef INC_COMM_POSTMAN_H_
#define INC_COMM_POSTMAN_H_

#include <stdint.h>
#include <stdbool.h>

#include "Comm/I2CComm_Generic.h"

/**
 * @brief The Max Size of Post Address List
 * 
 */
#define MaxSizePostArvEntryList	(0x100)

/**
 * @brief Thr Max Size of Post Message
 * 
 */
#define MaxSizePostMessage	150

/**
 * @brief The Protocol Version of this Document
 * 
 */
#define ProtocolVersion	0x01

/**
 * @brief The Possible Error Status will Occurred during Procedure
 * 
 */
typedef enum
{
	PostmanErrorType_None,
	PostmanErrorType_Registration,
	PostmanErrorType_UnRegistration,
	PostmanErrorType_RecvPost,
	PostmanErrorType_SendPost,
	PostmanErrorType_NotifyPost,
	PostmanErrorType_ReplyPost,
	PostmanErrorType_PostSize,
	PostmanErrorType_CRC,
	PostmanErrorType_NoReceiverExist,
	PostmanErrorType_CommFail,
}PostmanErrorType;

/**
 * @brief The Callback Function of Mail Matched the Address
 * 
 */
typedef void (* PostArvCallback)(uint8_t postAddr, uint8_t *msg, uint8_t msgSize);

/**
 * @brief Postman Service Initialization
 * 
 */
void PostmanInit(void);

/**
 * @brief Register the Address is going to Get Post
 * 
 * @param adrKey The Address is gong to be receive
 * @param entry The Notification Entry of Post Arrived
 * @return true The Operation is Valid
 * @return false The Operation is Invalid
 */
bool RegisterPostArvEntry(uint8_t adrKey, PostArvCallback entry);

/**
 * @brief Unregister the Address is going to Get Post
 * 
 * @param adrKey The Address is gong to be receive
 * @param entry The Notification Entry of Post Arrived
 * @return true The Operation is Valid
 * @return false The Operation is Invalid
 */
bool UnregisterPostArvEntry(uint8_t adrKey, PostArvCallback entry);

/**
 * @brief Apply to Reply Post to the Sender
 * 
 * @param command The Command of this Post
 * @param msg The Content of Post
 * @param size This Size of Content
 * @return true The Operation is Valid
 * @return false The Operation is Invalid
 */
bool ReplyPostToSender(uint8_t command, uint8_t *msg, uint8_t size);

/**
 * @brief The Main Task of Postman Service
 * 
 * @return PrcsRes_Processing The Procedure is Processing
 * @return PrcsRes_Fail The Procedure is Failed
 * @return PrcsRes_Success The Procedure is Success
 */
PrcsRes PostmanTask(void);

/**
 * @brief Get the Postman Error Status
 * 
 * @return uint32_t The Reason of Error Occurred
 */
uint32_t GetPostmanErrStatus(void);

#endif /* INC_COMM_POSTMAN_H_ */
