#include <6502.h>
#include <lynx.h>
#include <tgi.h>
#include <stdint.h>
#include <string.h>
#include "4x6font.h"
#include "drawfont.h"

#include "../standard_lib.h"

SCB_REHV_PAL char_sprite = {
    BPP_4 | TYPE_NORMAL,
    REHV,
    0x01,
    0,
    NULL,
    0, 0,
    0x0100, 0x0100,
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }		// only two pens used, background, white
};


// static variables to reduce code size
unsigned char **font;
unsigned char xc;
char *c;


void _outtext_font(uint8_t fg, uint8_t bg)
{
  // initialize
  while (*c) {
    // Character in printable range?
    if ((*c > 0x20) && (*c < 0x7E)) {
        char_sprite.data = font[*c-0x20];
        char_sprite.hpos = xc;
        char_sprite.penpal[0] = (fg << 4) + (bg);
    }
    else {
        char_sprite.data = font[0];         // space overwrite background
        char_sprite.hpos = xc;
        char_sprite.penpal[0] = (bg << 4);
    }

    tgi_sprite(&char_sprite);
    xc += FONT_X_SIZE;
    c++;
  }
}


// Output string at x, y screen location (Lynx screen coords)
void outtext_4x6(uint8_t x, uint8_t y, uint8_t fg, uint8_t bg, const char *s)
{
    c = s;
    xc = x;
    char_sprite.vpos = y;

    // set font
    font = &font4x6[0];

    _outtext_font(fg, bg);
}

