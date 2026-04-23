#ifndef DRAWFONT_H
#define DRAWFONT_H

#include "../standard_lib.h"

// Font size in pixels
#define FONT_X_SIZE	4
#define FONT_Y_SIZE	6

// char to screen position
#define CHAR_X_SCR(x) ((x) * FONT_X_SIZE)
#define CHAR_Y_SCR(y) ((y) * FONT_Y_SIZE)

// screen to char position
#define SCR_X_CHAR(x) ((x) / FONT_X_SIZE)
#define SCR_Y_CHAR(y) ((y) / FONT_Y_SIZE)

void outtext_4x6(uint8_t x, uint8_t y, uint8_t fg, uint8_t bg, const char *s);

#endif