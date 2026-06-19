/*
 * TILE PAINTER
 *
 * This -- eventually -- will be an app where I can design sprites and other tiles for 
 * my 2D games. It'll initially be pretty rigid, but I'll add more functionality as I go
 * (and as I need it). 
 *
 * I'm thinking I'll take some design points from NeoVim herself, specifically that whole
 * "modal" outlook, and the typed commands. Instead of some UI that could take millennia to write. 
 *
 * Also from nvim (and many others) I want to figure out how to make it a lil command-line command. And,
 * following that, the command-line input stuff (for example: nvim main.c include/game.h) so I can do more
 * fun stuff. 
 *
 * Right now, I just want a simple "canvas" in the middle, with everything set to black. If you click
 * on a tile, it swaps from black to white (or back). Then, on close, it'll print the output. That's all, for now. 
 *
 */

#include "NYFW/nyfw.h"
#include <unistd.h>
#include <stdio.h>


/* ----- TILE MODULE ----- */
int border_len = 20;	// length of TileModule border
int pixel_size = 75;

typedef struct {
	NYFW_Rect tile_rect;		// rect for the entire tile
	NYFW_Rect pixel_rects[64]; 	// rects for each pixel
	NYFW_Rect border_rects[4];	// rects for drawing the border

	uint16_t pixels[64];		// pixel values

	NYFW_Canvas scr;
} TileModule;


TileModule tile_mod_init(NYFW_Canvas scr)
{
	TileModule out;
	out.scr = scr;

	/* ----- SCREEN DIMENSIONS ----- */
	int sw = scr.width, 	sh = scr.height;		// screen dimensions
	int centerx = sw / 2, 	centery = sh / 2;		// center of the screen
	
	/* ----- TILE RECTS ----- */
	int rl_2 = pixel_size * 4;
	int rl = rl_2 * 2;	// width of the tile

	//int rl_2 = sh / 4;
	//int rl = rl_2 * 2;	// width of the tile	
	int rx = centerx - rl_2, 	ry = centery - rl_2;	// top-left corner of the tile
	
	out.tile_rect = (NYFW_Rect){ rx, ry, rl, rl };

	/* ----- BORDER RECTS ----- */
	int cvx = rx - border_len, 	cvy = ry - border_len;
	int cvl = rl + 2 * border_len;

	out.border_rects[0] = (NYFW_Rect){ cvx, cvy, cvl, border_len };				// top
	out.border_rects[1] = (NYFW_Rect){ cvx, cvy+border_len+rl, cvl, border_len };		// bottom
	out.border_rects[2] = (NYFW_Rect){ cvx, cvy+border_len, border_len, rl };			// left
	out.border_rects[3] = (NYFW_Rect){ cvx+border_len+rl, cvy+border_len, border_len, rl };	// right

	// TODO: add a text thing at the top
	

	/* ------ PIXEL ARRAY ----- */
	for (int i = 0; i < 64; i++)
		out.pixels[i] = 0x0000;
	
	/* ----- PIXEL RECTS ----- */
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			out.pixel_rects[j * 8 + i] = (NYFW_Rect){ rx + i * pixel_size, ry + j * pixel_size, pixel_size, pixel_size };
	
	return out;
}


void tile_mod_draw(TileModule t)
{
	for (int i = 0; i < 4; i++)
		nyfw_canvasFill(t.scr, LBLUE, &t.border_rects[i]);

	for (int i = 0; i < 64; i++)
		nyfw_canvasFill(t.scr, t.pixels[i], &t.pixel_rects[i]);
}







/* ----- INIT/CLOSE ----- */
int setup()
{
	if (!nyfw_windowInit()) return 0;

	if (!nyfw_inputInit(INPUT_MOUSE | INPUT_KEYS)) { 
		nyfw_windowClose();
		return 0; 
	}
}

void shutdown()
{
	nyfw_inputClose();
	nyfw_windowClose();
}




/* ----- MAIN ----- */
int main() 
{
	if (!setup()) return 1;
	
	NYFW_Canvas scr = nyfw_getWindowCanvas();
	TileModule tmod = tile_mod_init(scr);
	
	nyfw_canvasClear(scr);
	tile_mod_draw(tmod);
	nyfw_windowPresent();

	sleep(3);


	shutdown();
	return 0;
}




