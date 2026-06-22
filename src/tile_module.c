/*
 * TILE MODULE
 *
 * This is the "canvas" for the tile. This whole thang really just
 * draws and handles input and stuff for the tile module. 
 *
 */

#include "tpaint.h"


int border_len = 20;	// length of TileModule border
int pixel_size = 75;

typedef struct {
	NYFW_Rect tile_rect;		// rect for the entire tile
	NYFW_Rect pixel_rects[64]; 	// rects for each pixel
	NYFW_Rect border_rects[4];	// rects for drawing the border

	uint16_t pixels[64];	// gonna remove this

	NYFW_Canvas scr;
} TileModule;

TileModule tmod;

int tile_mod_init(NYFW_Canvas scr)
{
	tmod.scr = scr;

	/* ----- SCREEN DIMENSIONS ----- */
	int sw = scr.width, 	sh = scr.height;		// screen dimensions
	int centerx = sw / 2, 	centery = sh / 2;		// center of the screen
	
	/* ----- TILE RECTS ----- */
	int rl_2 = pixel_size * 4;
	int rl = rl_2 * 2;	// width of the tile
	int rx = centerx - rl_2, 	ry = centery - rl_2;	// top-left corner of the tile
	
	tmod.tile_rect = (NYFW_Rect){ rx, ry, rl, rl };

	/* ----- BORDER RECTS ----- */
	int cvx = rx - border_len, 	cvy = ry - border_len;
	int cvl = rl + 2 * border_len;

	tmod.border_rects[0] = (NYFW_Rect){ cvx, cvy, cvl, border_len };				// top
	tmod.border_rects[1] = (NYFW_Rect){ cvx, cvy+border_len+rl, cvl, border_len };			// bottom
	tmod.border_rects[2] = (NYFW_Rect){ cvx, cvy+border_len, border_len, rl };			// left
	tmod.border_rects[3] = (NYFW_Rect){ cvx+border_len+rl, cvy+border_len, border_len, rl };	// right

	// TODO: add a text thing at the top
	

	/* ------ PIXEL ARRAY ----- */
	for (int i = 0; i < 64; i++)
		tmod.pixels[i] = 0x0000;
	
	/* ----- PIXEL RECTS ----- */
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			tmod.pixel_rects[j * 8 + i] = (NYFW_Rect){ rx + i * pixel_size, ry + j * pixel_size, pixel_size, pixel_size };
	
	return 1;
}


void tile_mod_draw()
{
	for (int i = 0; i < 4; i++)
		nyfw_canvasFill(tmod.scr, LBLUE, &tmod.border_rects[i]);

	for (int i = 0; i < 64; i++)
		nyfw_canvasFill(tmod.scr, tile.pixels[i], &tmod.pixel_rects[i]);
}


#define IN_RECT(r, x, y)	((r).x <= (x) && (r).y <= (y) && (x) < (r).x + (r).w && (y) < (r).y + (r).h) ? 1 : 0

void tile_check_input(int x, int y)
{
	if (!IN_RECT(tmod.tile_rect, x, y)) return;
	

	for (int i = 0; i < 64; i++) {
		if (IN_RECT(tmod.pixel_rects[i], x, y)) {
			tile.pixels[i] = ~tmod.pixels[i];
			break;
		}
	}
}

NYFW_Canvas get_tile()
{
	uint16_t* p = tmod.pixels;
	return nyfw_canvas(p, 8, 8, 8);
}


