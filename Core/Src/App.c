/*
 * App.c
 *
 *  Created on: Aug 27, 2024
 *      Author: Sam
 */

#include "Display/ssd1306.h"
#include "TestFunc/TestFunc.h"

void AppInit(void)
{
	ssd1306_Init();
}

void AppTask(void)
{
	TestFuncTask();
}
