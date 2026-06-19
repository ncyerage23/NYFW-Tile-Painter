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
 *
 * TODO: what I have rn works, which is good. But, for some reason, it's unreasonably slow. There's probably a few
 * good reasons for that, but I haven't had problems like this ever. So...idk. Figure ts out. 
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
		nyfw_canvasFill(tmod.scr, tmod.pixels[i], &tmod.pixel_rects[i]);
}







/* ----- INIT/CLOSE ----- */
NYFW_Canvas scr;

int setup()
{
	if (!nyfw_windowInit()) return 0;
	scr = nyfw_getWindowCanvas();

	if (!nyfw_inputInit(INPUT_MOUSE | INPUT_KEYS)) { 
		nyfw_windowClose();
		return 0; 
	}

	if (!tile_mod_init(scr)) {
		nyfw_inputClose();
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

// mouse
int mx = 0, my = 0;

void draw_mouse()
{
	nyfw_canvSetPixel(scr, mx, my, 0xffff); 
}


int main() 
{
	if (!setup()) return 1;
	
	nyfw_canvasClear(scr);

	while (1) {
		nyfw_inputPoll();
		
		// update (TODO: make this a separate function)
		if (nyfw_inputKeyPressed(NYFW_KEY_ESC)) break;

		mx += nyfw_inputMouseDX();
		my += nyfw_inputMouseDY();

		// draw
		nyfw_canvasClear(scr);
		tile_mod_draw();
		draw_mouse();

		// present
		nyfw_windowPresent();
	}

	shutdown();
	return 0;
}




