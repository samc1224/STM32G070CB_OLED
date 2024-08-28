/*
 * Activation.h
 *
 *  Created on: Aug 28, 2024
 *      Author: Sam
 */

#ifndef INC_GENERIC_ACTIVATION_H_
#define INC_GENERIC_ACTIVATION_H_

#include "main.h"

typedef enum
{
	Button0,
	Button1,
	Button2,
	Button3,
	Button4,
	Button5,
	Button6,
	Button7,
	Button8,
}ButtonPin;

typedef enum
{
	LED0,
	LED1,
	LED2,
	LED3,
	LED4,
	LED5,
}LEDPin;

GPIO_PinState ReadButton(ButtonPin btn);
void WriteLED(LEDPin led, GPIO_PinState sta);

#endif /* INC_GENERIC_ACTIVATION_H_ */
