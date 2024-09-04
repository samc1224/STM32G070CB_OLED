/*
 * OLEDCtrl.h
 *
 *  Created on: Aug 27, 2024
 *      Author: Sam
 */

#ifndef INC_DISPLAY_OLEDCTRL_H_
#define INC_DISPLAY_OLEDCTRL_H_

#include <stdint.h>

void OLED_Reset(void);
void OLED_Clear(uint8_t color);
void OLED_ShowString_16x26W(uint8_t x, uint8_t y, char* str);
void OLED_ShowString_11x18W(uint8_t x, uint8_t y, char* str);
void OLED_ShowString_7x10W(uint8_t x, uint8_t y, char* str);
void OLED_ShowChar_11x18W(uint8_t x, uint8_t y, char ch);

#endif /* INC_DISPLAY_OLEDCTRL_H_ */
