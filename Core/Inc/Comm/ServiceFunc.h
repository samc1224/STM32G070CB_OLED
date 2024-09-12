/*
 * ServiceFunc.h
 *
 *  Created on: Sep 11, 2024
 *      Author: Sam
 */

#ifndef INC_COMM_SERVICEFUNC_H_
#define INC_COMM_SERVICEFUNC_H_

#include "Generic/Generic.h"

/* Service Mode Command (NanoPb not used) */
#define ServiceMode_Command	(0xFE)

typedef enum
{
	/* Service Mode Write Command */
	ServiceCmd_EnterServiceMode = 0,
	ServiceCmd_SetEc1Knob,
	ServiceCmd_SetEc2Knob,
	ServiceCmd_SetEc1Button,
	ServiceCmd_SetEc2Button,
	ServiceCmd_SetVirtualButton,
	ServiceCmd_SetResistorValue,
	ServiceCmd_WriteFWVer = 0x21,
	ServiceCmd_WriteHWVer,
	ServiceCmd_WriteSerialNumber,
	ServiceCmd_QuitServiceMode = 0x7F,

	/* Service Mode Read Command */
	ServiceCmd_GetResistorValue,
	ServiceCmd_ReadFWVer = 0xA1,
	ServiceCmd_ReadHWVer,
	ServiceCmd_ReadSerialNumber,
}ServiceCmdList;

/**
 * @brief Service Function Initialization
 *
 */
void ServFuncInit(void);

/**
 * @brief Service Function Entry
 *
 * @return PrcsRes Process Result
 */
PrcsRes ServiceFuncTask(void);

#endif /* INC_COMM_SERVICEFUNC_H_ */
