/*
 * OLEDCtrl.c
 *
 *  Created on: Aug 27, 2024
 *      Author: Sam
 */

#include <string.h>
#include <stdio.h>
#include "Display/OLEDCtrl.h"
#include "Display/ssd1306.h"
#include "Display/ssd1306_fonts.h"

// OLED can display the Y-axis up to 32, and will not display if it exceeds 32
void OLED_Clear(uint8_t color)
{
	ssd1306_Fill(color); // 0:Black, 1:White
}

void OLED_ShowString_16x26W(uint8_t x, uint8_t y, char* str)
{
	#ifdef SSD1306_INCLUDE_FONT_16x26
	ssd1306_SetCursor(x, y);
	ssd1306_WriteString(str, Font_16x26, White);
	ssd1306_UpdateScreen();
	#endif
}

void OLED_ShowString_11x18W(uint8_t x, uint8_t y, char* str)
{
	#ifdef SSD1306_INCLUDE_FONT_11x18
	ssd1306_SetCursor(x, y);
	ssd1306_WriteString(str, Font_11x18, White);
	ssd1306_UpdateScreen();
	#endif
}

void OLED_ShowString_7x10W(uint8_t x, uint8_t y, char* str)
{
	#ifdef SSD1306_INCLUDE_FONT_7x10
	ssd1306_SetCursor(x, y); // 22+10=32
	ssd1306_WriteString(str, Font_7x10, White);
	ssd1306_UpdateScreen();
	#endif
}

void OLED_ShowChar_11x18W(uint8_t x, uint8_t y, char ch)
{
	#ifdef SSD1306_INCLUDE_FONT_11x18
	ssd1306_SetCursor(x, y);
	ssd1306_WriteChar(ch, Font_11x18, White);
	ssd1306_UpdateScreen();
	#endif
}
