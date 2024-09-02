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
	Button0, // 0: Not used
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
	LED0, // 0: Not used
	LED1,
	LED2,
	LED3,
	LED4,
	LED5,
}LEDPin;

typedef enum
{
	Relay0, // 0: Not used
	Relay1,
	Relay2,
	Relay3,
	Relay4,
	Relay5,
	Relay6,
	Relay7,
	Relay8,
	Relay9,
	RelayOpen,
	RelayShort,
}RelayPin;

GPIO_PinState ReadButton(ButtonPin btn);
void WriteLED(LEDPin led, GPIO_PinState sta);
void WriteRelay(RelayPin rly, GPIO_PinState sta);
void LedAllOff(void);
void RelayAllOff(void);

#endif /* INC_GENERIC_ACTIVATION_H_ */
