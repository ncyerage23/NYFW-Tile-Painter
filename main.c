/*
 * TILE PAINTER
 *
 * This -- eventually -- will be an app where I can design sprites and other tiles for 
 * my 2D games. It'll initially be pretty rigid, but I'll add more functionality as I go
 * (and as I need it). 
 *
 * So, this version works! It might possibly be incredibly slow, but I can't really tell. 
 * Either way, it does work. Next step: saving to .nymg, then maybe palette module or 
 * the command-line commands (like ./tilepainter poo.nymg or something). Or text commands (ugh). 
 *
 * One thing. I think the input system might be super janky. Same with the window one. NYFW might be janky. 
 * So, another thing to figure out. TODO: try to allow for ^C quitting. 
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


#define IN_RECT(r, x, y)	((r).x <= (x) && (r).y <= (y) && (x) < (r).x + (r).w && (y) < (r).y + (r).h) ? 1 : 0

void tile_check_input(int x, int y)
{
	if (!IN_RECT(tmod.tile_rect, x, y)) return;
	

	for (int i = 0; i < 64; i++) {
		if (IN_RECT(tmod.pixel_rects[i], x, y)) {
			tmod.pixels[i] = ~tmod.pixels[i];
			break;
		}
	}
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
int mx = 10, my = 10;
int mspeedx = 5, mspeedy = 4;

void draw_mouse()
{
	// nyfw_canvSetPixel(scr, mx, my, 0xffff); 
	
	CANV_PIXEL(scr, mx, my) = 0xffff;
	CANV_PIXEL(scr, mx-1, my) = 0xffff;
	CANV_PIXEL(scr, mx+1, my) = 0xffff;
	CANV_PIXEL(scr, mx, my-1) = 0xffff;
	CANV_PIXEL(scr, mx, my+1) = 0xffff;

}


int main() 
{
	if (!setup()) return 1;
	
	int sw = scr.width, sh = scr.height;

	while (1) {
		nyfw_inputPoll();
		
		// update (TODO: make this a separate function)
		if (nyfw_inputKeyPressed(NYFW_KEY_ESC)) break;

		mx += nyfw_inputMouseDX() * mspeedx;
		my += nyfw_inputMouseDY() * mspeedy;
		
		mx = MAX(5, MIN(mx, sw-5));
		my = MAX(5, MIN(my, sh-5));

		if (nyfw_inputMBPressed(NYFW_MB_L)) tile_check_input(mx, my);

		// draw
		nyfw_canvasClear(scr);
		tile_mod_draw();
		draw_mouse();

		// present
		nyfw_windowPresent();
	}
	
	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < 8; i++)
			printf("%X\t", tmod.pixels[j * 8 + i]);
		printf("\n");
	}

	shutdown();
	return 0;
}




