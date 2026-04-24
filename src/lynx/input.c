#include <lynx.h>
#include <tgi.h>
#include <joystick.h>
#include <conio.h>

#include "drawfont.h"
#include "vars.h"
#include "../platform-specific/graphics.h"
#include "../misc.h"


#define INPUT_CHARS_NUM     36		// last array index
const char input_chars[] = {" abcdefghijklmnopqrstuvwxyz1234567890"};

unsigned char readJoystick()
{
    return(joy_read(0));
}


#ifdef USE_PLATFORM_SPECIFIC_INPUT

void getPlatformKey_anykey(void)
{
	unsigned char joy;

	// any button or keypress exits
	while(1) {
		if (kbhit()) {
			cgetc();
			return;
		}

        joy = joy_read(0);
        if (joy) {
            while (joy_read(0) == joy);     // debounce joystick
        }

      	if (JOY_BTN_1(joy) || JOY_BTN_2(joy))
            return;
	}
}


// Lynx keys are 1 = Opt1, 2 = Opt2, 3 = Opt1+Opt2, R = Opt1 + Pause, F = Opt2 + Pause
int getPlatformKey_screens(void)
{
	char c;

	// no key hit
	if (!kbhit()) {
		return(0);
	}

	c = cgetc();
	switch(c) {
		case '1':					// sound toggle
			return('S');
		case 'P':					// help
			return('H');
		case '2':					// refresh / in-game menu
			return('R');
		case '3':					// name
			return('N');
		case 'R':					// Quit the game
			return('Q');
		case 'F':					// flip the screen
			tgi_flip();
			break;
	}

	return(0);
}



int getPlatformKey_inputfield(int8_t x, int8_t y, int8_t at_max)
{
    unsigned char i, last;
    unsigned char joy;
    char s[2];


    i = 0;

	// hack so that backspace works properly
	drawBlank(x, y);
	drawIcon(x, y, ICON_TEXT_CURSOR);

    // keys input
    while(1) {
		last = i;

        // joystick input
        joy = joy_read(0);
        if (joy) {
            while (joy_read(0) == joy);     // debounce joystick
        }

		if (!at_max) {
        	if (JOY_DOWN(joy) || JOY_RIGHT(joy)) {
            	if (i != 0)
                	i--;
            	else
                	i = INPUT_CHARS_NUM;
        }

        if (JOY_UP(joy) || JOY_LEFT(joy)) {
            if (i < INPUT_CHARS_NUM)
                i++;
            else
                i = 0;
        }

		}

		// A button selects this character, or RETURN if no character selected
        if (JOY_BTN_1(joy)) {
            if (i != 0)
                return(input_chars[i]);
            else
                return(KEY_RETURN);
        }

		// B button sends backspace
        if (JOY_BTN_2(joy)) {
			drawBlank(x, y);
            return(KEY_BACKSPACE);
        }

		// Did we change character?
		if (last != i) {
			if (i == 0) {
				drawBlank(x, y);
				drawIcon(x, y, ICON_TEXT_CURSOR);
			}
			else {
				// Select the character from input_chars array
        		s[0] = input_chars[i];
        		s[1] = '\0';

				// draw the character on the screen
        		tgi_setcolor(lynx_bg_color);
        		tgi_bar(CHAR_X_SCR(x), CHAR_Y_SCR(y), CHAR_X_SCR(x)+FONT_X_SIZE, CHAR_Y_SCR(y)+FONT_Y_SIZE);
        		tgi_setcolor(lynx_text_color);
        		drawText(x, y, s);
			}
		}
	}
 }

// this function is main input function for joystick/arrows
// joystick up/down/left/right moves cursor
// joystick button A (place/attack)
// joystick button B (rotate ship)
// opt1 == ESC (menu)
void platform_readCommonInput()
{
    input.trigger = input.key = input.dirX = input.dirY = 0;

    _joy = readJoystick();

    // Simulate the keyboard delay for joystick input, by checking previous joystick value
    // There is special logic so that "shifting into a diagnal" still results in a single X and Y move
    // e.g. RIGHT, RIGHT+UP, UP moves the cursor the same as a single RIGHT+UP, rathar than TWO to the right and ONE up
    if (_joy != _lastJoy)
    {
        if (JOY_LEFT(_joy) && (_lastJoy == 99 || !JOY_LEFT(_lastJoy)))
            input.dirX = -1;
        else if (JOY_RIGHT(_joy) && (_lastJoy == 99 || !JOY_RIGHT(_lastJoy)))
            input.dirX = 1;

        if (JOY_UP(_joy) && (_lastJoy == 99 || !JOY_UP(_lastJoy)))
            input.dirY = -1;
        else if (JOY_DOWN(_joy) && (_lastJoy == 99 || !JOY_DOWN(_lastJoy)))
            input.dirY = 1;

        // Reset the delay if no movement is detected
        if (_joy == 0)
            _joySameCount = 12;

        _lastJoy = _joy;

        // Trigger button press only if it was previously unpressed
        if (JOY_BTN_1(_joy) || JOY_BTN_2(_joy))
        {
            if (JOY_BTN_1(_joy))
            	input.trigger = true;
            if (JOY_BTN_2(_joy)) {
            	input.key = 'R';
			}
        }

		// debounce joystick
		if (_joy !=0)
			while(_joy == readJoystick());
 	}

	if (input.key == 0)
 		input.key = getPlatformKey_screens();
	return;
}

#endif