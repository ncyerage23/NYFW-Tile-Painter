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


/* ----- TILE MODULE ----- */
NYFW_Canvas tile_module;
NYFW_Rect tile_rect;
NYFW_Canvas tiles[64];
int tile_x, tile_y;		// location (in scr) where tile canvas starts

void tile_init(NYFW_Canvas scr, NYFW_Rect tr);


// alright. I'm losing it a little bit. Need to go back to planning. See ya!




/* ----- CANVAS VIEW MODULE ----- */
NYFW_Canvas canvas_view;	// the entire module -- the sprite/tile being drawn, + the outline and things
NYFW_Rect canvas_view_rects[4];
int border_len = 20;


void canvas_view_init(NYFW_Canvas scr)
{
	/* ----- SCREEN DIMENSIONS ----- */
	int sw = scr.width, 	sh = scr.height;		// screen dimensions
	int centerx = sw / 2, 	centery = sh / 2;		// center of the screen
	
	/* ----- TILE DIMENSIONS ----- */
	int rl_2 = sh / 4;
	int rl = rl_2 * 2;	// width of the tile	
	int rx = centerx - rl_2, 	ry = centery - rl_2;	// top-left corner of the tile
	
	// TODO: call sum to init the tile
	

	/* ----- CANVAS VIEW DIMENSIONS ----- */
	int cvx = rx - border_len, 	cvy = ry - border_len;
	int cvl = rl + 2 * border_len;

	/* ----- CANVAS VIEW ----- */
	NYFW_Rect canvas_view_rect = { cvx, cvy, cvl, cvl };
	canvas_view = nyfw_subcanvas(scr, &canvas_view_rect);

	/* ----- CANVAS VIEW BORDER RECTS ----- */
	canvas_view_rects[0] = (NYFW_Rect){ 0, 0, cvl, border_len };				// top
	canvas_view_rects[1] = (NYFW_Rect){ 0, border_len+rl, cvl, border_len };		// bottom
	canvas_view_rects[2] = (NYFW_Rect){ 0, border_len, border_len, rl };			// left
	canvas_view_rects[3] = (NYFW_Rect){ border_len+rl, border_len, border_len, rl };	// right


	// TODO: add a text thing at the top
}


void canvas_view_draw()
{
	for (int i = 0; i < 4; i++)
		nyfw_canvasFill(canvas_view, 0xffff, &canvas_view_rects[i]);

	// TODO: text thing -- and maybe call tile draw

}




/* ----- MAIN ----- */
int main() 
{
	if (!setup()) return 1;
	
	NYFW_Canvas scr = nyfw_getWindowCanvas();
	canvas_view_init(scr);

	nyfw_canvasClear(scr);
	canvas_view_draw();
	nyfw_windowPresent();

	sleep(5);


	shutdown();
	return 0;
}




