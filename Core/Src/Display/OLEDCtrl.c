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

void OLED_ShowString_11x18(char* str)
{
    ssd1306_Fill(Black);

	#ifdef SSD1306_INCLUDE_FONT_11x18
	ssd1306_SetCursor(2, 0);
	ssd1306_WriteString(str, Font_11x18, White);
	ssd1306_UpdateScreen();
	#endif
}
