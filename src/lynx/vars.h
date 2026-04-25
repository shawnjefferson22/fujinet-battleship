#ifndef LYNX_VARS_H
#define LYNX_VARS_H

#include <joystick.h>
#include "../standard_lib.h"

#define CUSTOM_FUJINET_CALLS

// Platform Specific functions for different input
#define USE_PLATFORM_SPECIFIC_INPUT
void getPlatformKey_anykey(void);
int getPlatformKey_screens(void);
int getPlatformKey_inputfield(int8_t x, int8_t y, int8_t at_max);
void platform_readCommonInput();
void platformStatusKeyLegend();
void platformMenuScreen();
uint8_t lynx_timer_x();

// Screen dimensions for platform
#define WIDTH 40
#define HEIGHT 17
#define PLAYER_BOX_TOP 10
#define PLACE_TEXTPLACE_TEXT_KEY_PROMPT "press B to rotate"
#define TABLE_CHOOSE_Y 3
#define TABLE_TOP 5
#define TABLE_SPACING 1
#define TABLE_HELLO_Y 14
#define LOBBY_TABLE_TOP 4
#define LOBBY_KEYHELP_y 14
#define LOBBY_PROMPT_Y 12
#define CONNECTING_Y 12
#define PLACE_TEXT_Y 1
#define PLACE_TEXT_SPACING 1
#define TIMER_WIDTH 2
#define TIMER_X		lynx_timer_x()

// color globals
extern unsigned lynx_bg_color;
extern unsigned lynx_text_color;
extern unsigned lynx_alttext_color;

// Other platform specific constants
#define QUERY_SUFFIX "" // No extra params for Atari

// Icons (these will be sprites)
#define ICON_TEXT_CURSOR	0		// used in gamelogic.c
#define ICON_MARK			1		// used in gamelogic.c / screens.c (arrow / selection marker)
#define ICON_MARK_ALT		2		// used in screens.c
#define ICON_PLAYER			3		// used in screens.c (icon to depict players in this game)
#define ICON_CURSOR			4
#define ICON_CURSOR_ALT		5

/**
 * Platform specific key map for common input
 */
// these are used
#define KEY_ESCAPE          '2'
#define KEY_RETURN          1
#define KEY_BACKSPACE       2

// these are dummy values
#define KEY_ESCAPE_ALT      'Q'
#define KEY_SPACEBAR        'C'
#define KEY_LEFT_ARROW      'D'
#define KEY_LEFT_ARROW_2    'E'
#define KEY_LEFT_ARROW_3    'F'
#define KEY_RIGHT_ARROW     'G'
#define KEY_RIGHT_ARROW_2   'H'
#define KEY_RIGHT_ARROW_3   'I'
#define KEY_UP_ARROW        'J'
#define KEY_UP_ARROW_2      'K'
#define KEY_UP_ARROW_3      'L'
#define KEY_DOWN_ARROW      'M'
#define KEY_DOWN_ARROW_2    'N'
#define KEY_DOWN_ARROW_3    'O'


#endif