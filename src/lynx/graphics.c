#include <lynx.h>
#include <tgi.h>
#include <joystick.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

#include "../standard_lib.h"
#include "../misc.h"

#include "drawfont.h"
#include "sprites.h"
#include "vars.h"



unsigned lynx_bg_color;
unsigned lynx_text_color;
unsigned lynx_alttext_color;

// macros to set penpal in sprites (lo/hi nibble)
#define SET_LO(x, v)  (((x) & 0xF0) + ((v) & 0x0F))
#define SET_HI(x, v)  (((x) & 0x0F) + (((v) & 0x0F) << 4))

// Player locations and offsets for 2, 3 and 4 player
#define CELL_SIZE_2P		5
#define CELL_SIZE_34P		4
#define GRID_SIZE_2P		(CELL_SIZE_2P * 10)
#define GRID_SIZE_34P		(CELL_SIZE_34P * 10)

// game screen details
unsigned char grid_size;			// grid size, 40 or 50 (x/y dimension)
unsigned char cell_size;			// cell size, 4 or 5
unsigned char name_size_y;			// size of name area in frame
unsigned char lynx_max_players;		// num players in this game

// positions of data in pos arrays
#define PLR_X		0
#define PLR_Y		1
#define PLR_GRID_X	2
#define PLR_GRID_Y	3
#define PLR_DRAW_X	4
#define PLR_DRAW_Y	5
#define PLR_NAME_X	6
#define PLR_NAME_Y	7

// offsets to add to flipped frames (since they are drawn right to left)
#define FLIP_2P_ADD		78
#define FLIP_4P_ADD		67

// player position macros)
#define GRID_X(p)	((player_pos[(p)][PLR_X]+player_pos[(p)][PLR_GRID_X]))
#define GRID_Y(p)	((player_pos[(p)][PLR_Y]+player_pos[(p)][PLR_GRID_Y]))
#define DRAWER_X(p)	((player_pos[(p)][PLR_X]+player_pos[(p)][PLR_DRAW_X]))
#define DRAWER_Y(p)	((player_pos[(p)][PLR_Y]+player_pos[(p)][PLR_DRAW_Y]))
#define NAME_X(p)	((player_pos[(p)][PLR_X]+player_pos[(p)][PLR_NAME_X]))
#define NAME_Y(p)	((player_pos[(p)][PLR_Y]+player_pos[(p)][PLR_NAME_Y]))

// player positions and offsets for 1-4 players
uint8_t (*player_pos)[8];
									//PX, PY, GX, GY, DX, DY, NX, NY
unsigned char lynx_1p_pos[1][8]	=	{{35, 18, 27, 11,  2, 14, 27, 2}};

unsigned char lynx_2p_pos[2][8] =	{{ 0, 18, 27, 11,  2, 14, 27, 2},
									 {81, 18,  2, 11, 54, 14,  2, 2}};

unsigned char lynx_3p_pos[3][8] =	{{36, 51, 24,  9,  2, 11, 24, 2},
									 { 4,  0, 24,  9,  2, 11, 24, 2 },
									 {88,  0,  4,  9, 46, 11,  4, 2 }};

unsigned char lynx_4p_pos[4][8] = 	{{ 4, 51, 24,  9,  2, 11, 24, 2 },
									 { 4,  0, 24,  9,  2, 11, 24, 2 },
									 {88,  0,  4,  9, 46, 11,  4, 2 },
									 {88, 51,  4,  9, 46, 11,  4, 2 }};

// drawer offsets for frames
uint8_t	drawer_2p_ship_pos[5][2] = {{2, 3}, { 9, 3}, {16, 3}, {16, 25}, { 9, 30}};
uint8_t drawer_4p_ship_pos[5][2] = {{2, 2}, { 8, 2}, {14, 2}, {14, 20}, { 8, 24}};
uint8_t (*drawer_ship_pos)[2];



// Call to clear the screen
void resetScreen()
{
    tgi_setcolor(lynx_bg_color);
    tgi_clear();
}

uint8_t cycleNextColor()
{
	return 0;
}

/// @brief Draw a string of text, default color
void drawText(uint8_t x, uint8_t y, const char *s)
{
	outtext_4x6(CHAR_X_SCR(x), CHAR_Y_SCR(y), lynx_text_color, lynx_bg_color, s);
}

/// @brief Draw a string of text. Draw alternate color for CAPITALIZED letters.
void drawTextAlt(uint8_t x, uint8_t y, const char *s)
{
  	uint8_t sx, sy;
	char c[2];


	sy = CHAR_Y_SCR(y);		// initial screen coords
	sx = CHAR_X_SCR(x);
	c[1] = '\0';			// null terminate the string

	while (*s) {
        c[0] = *s;

        outtext_4x6(sx, sy, islower(c[0]) ? lynx_text_color : lynx_alttext_color, lynx_bg_color, c);

        sx += FONT_X_SIZE;
        s++;
    }
}

/// @brief Draw a single character - used for icons
void drawIcon(uint8_t x, uint8_t y, uint8_t icon)
{
	SCB_REHV_PAL *sprite;

	switch(icon) {
		case ICON_TEXT_CURSOR:			// cursor for entering name
			sprite = &text_cursor_sprite;
			break;
		case ICON_MARK:					// arrow next to selection
			sprite = &mark_sprite;
			sprite->penpal[0] = SET_LO(ship_sprite.penpal[0], TGI_COLOR_BLACK);
			break;
		case ICON_MARK_ALT:				// 	flash color of mark sprite
			sprite = &mark_sprite;
			sprite->penpal[0] = SET_LO(ship_sprite.penpal[0], TGI_COLOR_RED);
			break;
		case ICON_PLAYER:				// icon to denote players in this game
			sprite = &player_sprite;
			break;
	}

	// draw the sprite
	sprite->hpos = CHAR_X_SCR(x);
	sprite->vpos = CHAR_Y_SCR(y);
	tgi_sprite(sprite);
}

/// @brief Draw (or erase) a ship at given size and position.
/// @param quadrant player index (0-3) for positioning purpose
/// @param size ship size (2-5)
/// @param pos ship position 0-99, add 100 for vertical
/// @param hide 0=show, 1=hide
void drawShip(uint8_t quadrant, uint8_t size, uint8_t pos, bool hide)
{
	uint8_t sx, sy;
	uint8_t rp;
	uint8_t vert;

	// real position
	rp = (pos > 99) ? (pos - 100) : pos;
	vert = (pos > 99);

	// calcuate cell screen coords
	sx = GRID_X(quadrant) + ((rp % 10) * cell_size);
	sy = GRID_Y(quadrant) + ((rp / 10) * cell_size);

	// Erase the ship?
	if (hide) {
		tgi_setcolor(TGI_COLOR_LIGHTBLUE);
		if (vert)
			tgi_bar(sx, sy, sx+cell_size, sy+(size*cell_size));
		else
			tgi_bar(sx, sy, sx+(size*cell_size), sy+cell_size);

		return;
	}

	// large or small size of grids for ship sprite
	if (grid_size == GRID_SIZE_34P) {
		if (vert)
			ship_sprite.data = ship_2p_v_data[size-2];
		else
			ship_sprite.data = ship_2p_h_data[size-2];
	}
	else {
		if (vert)
			ship_sprite.data = ship_4p_v_data[size-2];
		else
			ship_sprite.data = ship_4p_h_data[size-2];
	}

	// draw the sprite
	ship_sprite.penpal[0] = SET_LO(ship_sprite.penpal[0], TGI_COLOR_GREY);
	ship_sprite.hpos = sx;
	ship_sprite.vpos = sy;
	tgi_sprite(&ship_sprite);
}

/// @brief Draw a ship either in whole or destroyed, in the legend (side trays)
/// @param player player index (0-3) for positioning purpose
/// @param index ship index (0-4)
/// @param size ship size (2-5)
/// @param status ship status: 0=destroyed, 1=intact
void drawLegendShip(uint8_t player, uint8_t index, uint8_t size, uint8_t status)
{
	// large or small size of grids for ship sprite
	if (grid_size == 50) {
		ship_sprite.data = ship_2p_v_data[size-2];
	}
	else {
		ship_sprite.data = ship_4p_v_data[size-2];
	}

	// ship intact or destroyed (grey or red)
	if (!status)
		ship_sprite.penpal[0] = SET_LO(ship_sprite.penpal[0], TGI_COLOR_RED);
	else
		ship_sprite.penpal[0] = SET_LO(ship_sprite.penpal[0], TGI_COLOR_GREY);

	// index is the ship index ? (0-4) is location    5, 4, 3, 3, 2 in order
	ship_sprite.hpos = DRAWER_X(player) + drawer_ship_pos[index][0];		// x position
	ship_sprite.vpos = DRAWER_Y(player) + drawer_ship_pos[index][1];		// y position

	tgi_sprite(&ship_sprite);
}

/// @brief Draw the player's name by their gamefield
/// @param player player index (0-3) for positioning purpose
/// @param name player name
/// @param active whether this is the currently active player
void drawPlayerName(uint8_t player, const char *name, bool active)
{
	uint8_t bg;

	// clear name area, make yellow for active, green for not active
	if (active)
		bg = TGI_COLOR_YELLOW;
	else
		bg = TGI_COLOR_LIGHTGREEN;

	tgi_setcolor(bg);
	tgi_bar(NAME_X(player), NAME_Y(player), NAME_X(player)+grid_size-1, NAME_Y(player)+name_size_y);

	// draw the name, smaller name area for 3/4 players
	if (grid_size == GRID_SIZE_2P)
		outtext_4x6(NAME_X(player)+4, NAME_Y(player)+1, TGI_COLOR_BLACK, TGI_COLOR_TRANSPARENT, name);
	else
		outtext_4x6(NAME_X(player)+4, NAME_Y(player), TGI_COLOR_BLACK, TGI_COLOR_TRANSPARENT, name);
}

/// @brief Draw end game message
/// @param message
void drawEndgameMessage(const char *message)
{
	// depending on 2p or 3/4p put message at bottom
	// or in the center

	outtext_4x6(0, 96, TGI_COLOR_WHITE, TGI_COLOR_BLACK, message);
}

/// @brief Draw the gamefield for a given player/quadrant
/// @param quadrant player index (0-3). Starting bottom left and moving clockwise
/// @param field pointer to 100 byte gamefield array from server
void drawGamefield(uint8_t quadrant, uint8_t *field)
{
	uint8_t i;
	uint8_t qx, qy;
	SCB_REHV_PAL *sprite;


	// clear to sea color
	tgi_setcolor(TGI_COLOR_LIGHTBLUE);
	tgi_bar(GRID_X(quadrant), GRID_Y(quadrant), GRID_X(quadrant)+grid_size-1, GRID_Y(quadrant)+grid_size-1);

	// get base xy of quadrant
	qx = GRID_X(quadrant);
	qy = GRID_Y(quadrant);

	// get correct sprites for grid size
	if (lynx_max_players < 3) {
		miss_sprite.data = &miss_2p_spr[0];
		hit_sprite.data = &hit_2p_1_spr[0];
	}
	else {
		miss_sprite.data = &miss_4p_spr[0];
		hit_sprite.data = &hit_4p_1_spr[0];
	}

	// draw sprites depending on field
	for (i=0; i<100; i++) {
		if (field[i] == FIELD_MISS)
			sprite = &miss_sprite;
		else if (field[i] == FIELD_ATTACK)
			sprite = &hit_sprite;
		else
			continue;

		// fall through to draw the sprite in cell
		sprite->hpos = qx + ((i % 10) * cell_size);
		sprite->vpos = qy + ((i / 10) * cell_size);
		tgi_sprite(sprite);
	}
}

/// @brief Draw/update a single cell (attackPos) for the the specified gamefield
/// @param quadrant [0-3] player index
/// @param gamefield pointer to 100 byte gamefield array from server
/// @param attackPos [0-99] position of cell to update
/// @param anim [0/1,10-15] : [0/1] toggle between two "hit" sprites for animation, [10-15] attack animation
void drawGamefieldUpdate(uint8_t quadrant, uint8_t *gamefield, uint8_t attackPos, uint8_t anim)
{
	uint8_t cx, cy;
	uint8_t cell;
	SCB_REHV_PAL *sprite;


	// calcuate cell screen coords
	cx = GRID_X(quadrant) + ((attackPos % 10) * cell_size);
	cy = GRID_Y(quadrant) + ((attackPos / 10) * cell_size);

	// get the gamefield cell
	cell = gamefield[attackPos];
	if (cell == 0)						// sanity check, nothing to draw?
		return;

	// clear to sea color
	tgi_setcolor(TGI_COLOR_LIGHTBLUE);
	tgi_bar(cx, cy, cx+cell_size-1, cy+cell_size-1);

	// get correct sprites for grid size
	if (lynx_max_players < 3) {
		miss_sprite.data = &miss_2p_spr[0];
		hit_sprite.data = hit_2p_data[anim];
		if (anim > 9)
			attack_sprite.data = attack_2p_data[anim-10];
	}
	else {
		miss_sprite.data = &miss_4p_spr[0];
		hit_sprite.data = hit_4p_data[anim];
		if (anim > 9) {
			attack_sprite.data = attack_4p_data[anim-10];
	}

	// change colors on last attack sprite (to red/yellow for fire)
	if (anim == 15)
		attack_sprite.penpal[0] = 0x29;
	else
		attack_sprite.penpal[0] = 0x15;
	}

	if (cell == FIELD_ATTACK) {
		if (anim > 9)
			sprite = &attack_sprite;
		else
			sprite = &hit_sprite;
	}
	else if (cell == FIELD_MISS)
		sprite = &miss_sprite;
	else
		return;

	// draw the sprite
	sprite->hpos = cx;
	sprite->vpos = cy;
	tgi_sprite(sprite);
}

/// @brief  Draw the cursor at the specified position on the specified gamefield
/// @param quadrant     [0-3] player index
/// @param x    [0-9] cursor x position
/// @param y    [0-9] cursor y position
/// @param gamefield pointer to 100 byte gamefield array from server
/// @param blink [0-2] used to cycle between different cursor sprites
void drawGamefieldCursor(uint8_t quadrant, uint8_t x, uint8_t y, uint8_t *gamefield, uint8_t blink)
{
	uint8_t cx, cy;

	// calcuate cell screen coords
	cx = GRID_X(quadrant) + (x * cell_size);
	cy = GRID_Y(quadrant) + (y * cell_size);

	// get correct sprites for grid size
	if (lynx_max_players < 3)
		cursor_sprite.data = cursor_2p_data[blink];
	else
		cursor_sprite.data = cursor_4p_data[blink];

	// change color for blink value of 2
	if (blink == 2)
		cursor_sprite.penpal[0] = (TGI_COLOR_YELLOW << 4);
	else
		//cursor_sprite.penpal[0] = SET_HI(ship_sprite.penpal[0], TGI_COLOR_WHITE);
		cursor_sprite.penpal[0] = (TGI_COLOR_WHITE << 4);

	// draw the sprite
	cursor_sprite.hpos = cx;
	cursor_sprite.vpos = cy;
	tgi_sprite(&cursor_sprite);

	(void)gamefield;		// avoid compiler error about unused var
}

/// @brief Draw the clock icon at the bottom right of the screen
void drawClock()
{
	// Draw a little clock icon sprite at bottom right
	// draw at bottom right if 2 player
	// draw somewhere else if 3/4 player (middle bottom?)
}

/// @brief Draw or erase the connection status icon at the bottom left of the screen
void drawConnectionIcon(bool show)
{
	// draw a little connection sprite at bottom left
}

/// @brief Draw a single blank space character at given position
void drawBlank(uint8_t x, uint8_t y)
{
    tgi_setcolor(lynx_bg_color);
	tgi_bar(CHAR_X_SCR(x), CHAR_Y_SCR(y), CHAR_X_SCR(x)+FONT_X_SIZE, CHAR_Y_SCR(y)+FONT_Y_SIZE);
}

/// @brief Draw W blank space characters at given position
void drawSpace(uint8_t x, uint8_t y, uint8_t w)
{
    tgi_setcolor(lynx_bg_color);
	tgi_bar(CHAR_X_SCR(x), CHAR_Y_SCR(y), CHAR_X_SCR(x)+(FONT_X_SIZE * w), CHAR_Y_SCR(y)+FONT_Y_SIZE);
}

/// @brief Draw a horizontal line of width W characters at location
void drawLine(uint8_t x, uint8_t y, uint8_t w)
{
	tgi_setcolor(lynx_text_color);
	tgi_line(CHAR_X_SCR(x), CHAR_Y_SCR(y), CHAR_X_SCR(x)+(FONT_X_SIZE * w), CHAR_Y_SCR(y));
}

/// @brief Draw a box at given location
void drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    //tgi_setcolor(lynx_fg_color);
	tgi_line(CHAR_X_SCR(x), CHAR_Y_SCR(y), CHAR_X_SCR(x)+(FONT_X_SIZE * w), CHAR_Y_SCR(y));
	tgi_lineto(CHAR_X_SCR(x)+(FONT_X_SIZE * w), CHAR_Y_SCR(y)+(FONT_Y_SIZE * h));
	tgi_lineto(CHAR_X_SCR(x), CHAR_Y_SCR(y)+(FONT_Y_SIZE * h));
	tgi_lineto(CHAR_X_SCR(x), CHAR_Y_SCR(y));
}

/// @brief Draw the main game board layout for the specified number of players
/// The player count dictates the general layout
void drawBoard(uint8_t playerCount)
{
	uint8_t i;
	uint8_t flip, flip_add;

	// draw 1-4 frames, depending on playercount
	// Setup all our player positions and offsets
	lynx_max_players = playerCount;			// record for later
	switch (playerCount) {
		case 1:
			//player_pos = &lynx_1p_pos;
			player_pos = (uint8_t (*)[8])lynx_1p_pos;
			break;
		case 2:
			//player_pos = &lynx_2p_pos;
			player_pos = (uint8_t (*)[8])lynx_2p_pos;
			break;
		case 3:
			//player_pos = &lynx_3p_pos;
			player_pos = (uint8_t (*)[8])lynx_3p_pos;
			break;
		case 4:
			//player_pos = &lynx_4p_pos;
			player_pos = (uint8_t (*)[8])lynx_4p_pos;
			break;
	}

	// setup sprites for 2 or 4 player game
	if (lynx_max_players < 3) {				// 5x5 grid size
		cell_size = CELL_SIZE_2P;
		grid_size = GRID_SIZE_2P;
		name_size_y = 7;
		drawer_ship_pos = drawer_2p_ship_pos;
		frame_sprite.data = &frame_2p_spr[0];
		flip_add = FLIP_2P_ADD;
	}
	else {									// 4x4 grid size
		cell_size = CELL_SIZE_34P;
		grid_size = GRID_SIZE_34P;
		name_size_y = 5;
		drawer_ship_pos = drawer_4p_ship_pos;
		frame_sprite.data = &frame_4p_spr[0];
		flip_add = FLIP_4P_ADD;
	}

	// draw player frames
	for (i=0; i<lynx_max_players; i++)
	{
		// for quadrant 2 or 3 we need to flip the frames horizontally
		if ((i == 1) && (lynx_max_players == 2))
			flip = 1;
		else if (i > 1)
			flip = 1;
		else
			flip = 0;

		// is this sprite flipped?
		if (flip) {
			frame_sprite.sprctl0 |= HFLIP;
			frame_sprite.hpos = player_pos[i][PLR_X] + flip_add;
		}
		else {
			frame_sprite.sprctl0 &= ~HFLIP;
			frame_sprite.hpos = player_pos[i][PLR_X];
		}

		frame_sprite.vpos = player_pos[i][PLR_Y];
		tgi_sprite(&frame_sprite);
	}
}

/// @brief Call to save screen buffer for later restore. Returns false if screen buffer not supported
bool saveScreenBuffer()
{
	return false;
}

/// @brief Call to restore screen buffer
void restoreScreenBuffer()
{
}

/// @brief Initialize graphics mode
void initGraphics()
{
	// Setup TGI
    tgi_install(tgi_static_stddrv);
    tgi_init();
    tgi_setcolor(TGI_COLOR_WHITE);

    // setup joystick - seems the best place to do this
    joy_install(joy_static_stddrv);

    // Init Fujinet - since we don't have Lynx in network-lib yet
    //fnio_init();

    // setup for double buffering
    tgi_setviewpage(0);
    tgi_setdrawpage(0);

    // Initial color mode
	lynx_bg_color = TGI_COLOR_BLACK;
	lynx_text_color = TGI_COLOR_WHITE;
	lynx_alttext_color = TGI_COLOR_YELLOW;
}

/// @brief Reset graphics mode to default state
void resetGraphics()
{

}

/// @brief Wait for vertical sync
void waitvsync()
{
	unsigned long t;

	t = clock();
 	while (t == clock());
}

void platformStatusKeyLegend()
{
				  			  //0123456789012345678901234567890123456789
	drawTextAlt(0, HEIGHT - 1, "2: REFRESH  P: HELP  1: COLOR  1+2: NAME");
}


void platformMenuKeys()
{
	drawBox(8, 4, 24, 8);
					//0123456789012345678901
	drawText(10, 6,  "RESTART: QUIT TABLE");
	drawText(10, 7,  "PAUSE:   HOW TO PLAY");
	drawText(10, 8,  "OPTION1: CHANGE COLORS");
  	drawText(10, 9,  "FLIP:    FLIP SCREEN");
	drawText(10, 11, "OPTION2: KEEP PLAYING");
 }