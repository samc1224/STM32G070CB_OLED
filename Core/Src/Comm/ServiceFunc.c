/*
 * ServiceFunc.c
 *
 *  Created on: Sep 11, 2024
 *      Author: Sam
 */

#include "Comm/ServiceFunc.h"

#include "Comm/Postman.h"
#include "Generic/Generic.h"
#include "Generic/Encoder.h"
#include "Generic/Activation.h"

static AppAttribute_t appAttr;
static EncoderParam_t encParam;

static void Post_ServiceFunctionHandler(uint8_t postAddr, uint8_t *msg, uint8_t msgSize);

void ServFuncInit(void)
{
	RegisterPostArvEntry(ServiceMode_Command, Post_ServiceFunctionHandler);
}

static void Post_ServiceFunctionHandler(uint8_t postAddr, uint8_t *msg, uint8_t msgSize)
{
	uint8_t replyPack[2];
	uint8_t servCmd = *msg;			// Service Write/Read Commands
	uint8_t servParam = *(msg + 1); // Service Param/Size

	switch(servCmd)
	{
		// -- Service Write Commands --
		case ServiceCmd_EnterServiceMode:
			break;
		case ServiceCmd_SetEc1Knob:
			RawValueSmallChange(servParam);
			ShowEncoderCount();
			break;
		case ServiceCmd_SetEc2Knob:
			RawValueBigChange(servParam);
			ShowEncoderCount();
			break;
		case ServiceCmd_SetEc1Button:
			ChangeEncoderSmallMultiple(servParam);
			break;
		case ServiceCmd_SetEc2Button:
			ChangeEncoderBigMultiple(servParam);
			break;
		case ServiceCmd_SetVirtualButton:
			SetVirtualButton(servParam);
			break;
		//
		case ServiceCmd_WriteFWVer:
			if(msgSize < 4)
				break;
			WriteFwVersionToRsvFlash(msg + 1);
			break;
		case ServiceCmd_WriteHWVer:
			if(msgSize < 4)
				break;
			WriteHwVersionToRsvFlash(msg + 1);
			break;
		case ServiceCmd_WriteSerialNumber:
			if(msgSize < 11)
				break;
			WriteSerialNumberToRsvFlash(msg + 1);
			break;

		// -- Service Read Commands --
		case ServiceCmd_GetResistorValue:
			encParam = ReadEncoderParam();
			encParam.cntRawValue *= 50;
			replyPack[0] = encParam.cntRawValue / 256;
			replyPack[1] = encParam.cntRawValue % 256;
			ReplyPostToSender(postAddr, replyPack, sizeof(replyPack));
			break;
		case ServiceCmd_ReadFWVer:
			appAttr = ReadRsvFlash();
			ReplyPostToSender(postAddr, appAttr.FWVersion, sizeof(appAttr.FWVersion));
			break;
		case ServiceCmd_ReadHWVer:
			appAttr = ReadRsvFlash();
			ReplyPostToSender(postAddr, appAttr.HWVersion, sizeof(appAttr.HWVersion));
			break;
		case ServiceCmd_ReadSerialNumber:
			appAttr = ReadRsvFlash();
			ReplyPostToSender(postAddr, appAttr.SerialNumber, sizeof(appAttr.SerialNumber));
			break;
	}
}
