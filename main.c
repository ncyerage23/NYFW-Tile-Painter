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
float m_sensitivity_y = 1.5;
float m_sensitivity_x = 2.0;

void undraw_mouse()
{
	nyfw_canvSetPixel(scr, mx, my, 0);
	nyfw_canvSetPixel(scr, mx-1, my, 0);
	nyfw_canvSetPixel(scr, mx+1, my, 0);
	nyfw_canvSetPixel(scr, mx, my-1, 0);
	nyfw_canvSetPixel(scr, mx, my+1, 0);

}


void draw_mouse()
{
	nyfw_canvSetPixel(scr, mx, my, 0xffff);
	nyfw_canvSetPixel(scr, mx-1, my, 0xffff);
	nyfw_canvSetPixel(scr, mx+1, my, 0xffff);
	nyfw_canvSetPixel(scr, mx, my-1, 0xffff);
	nyfw_canvSetPixel(scr, mx, my+1, 0xffff);

}


int main(int argc, char* argv[]) 
{
	// TODO: okay. This is basically good, minus the lack of colors. That'll come in a minute. 
	// Here's the plan. I'm gonna use command-line arguments for the file being edited. If the file exists, 
	// then we open the .nymg (unless it's not 8x8, I guess) and allow editing. If it doesn't, we start blank, 
	// and on close, save it to that name. 
	//
	// TODO: also, gonna need to clean this bitch up quite a bit. Before I do the ^ thing. Put the tile module stuff
	// in a different file, and make a header for everything (including the future palette module). this file should (obv)
	// just be for main stuff. That's it. init-update-draw, and the mainloop. 
	//
	// Idk. Should be a bit of work, but nothing too bad, right? Lol. When I start trying to do text input arguments, that's gonna
	// be painful. Hmm. Maybe doing getchar input would be better? Like, not using the mouse (except for special occasions...like a mode)
	// I mean, it'd be easier. Ooh! Maybe, when pressing a specific key (like ":") it exits mouse input mode, and does getchar. That could work. yeah!
	// Figure that out in a bit. After I get the rest of this bs done. 


	if (argc > 1) printf("%s\n", argv[1]);

	if (!setup()) return 1;
	
	int sw = scr.width, sh = scr.height;
	nyfw_canvasClear(scr);

	while (1) {
		nyfw_inputPoll();
		
		// update (TODO: make this a separate function)
		if (nyfw_inputKeyPressed(NYFW_KEY_ESC)) break;

		int mdx = (float)nyfw_inputMouseDX() * m_sensitivity_x;
		int mdy = (float)nyfw_inputMouseDY() * m_sensitivity_y;

		if (nyfw_inputMBPressed(NYFW_MB_L)) tile_check_input(mx, my);

		// draw (TODO: make this also a separate function)
		// nyfw_canvasClear(scr);
		undraw_mouse();
		tile_mod_draw();
		mx += mdx;
		my += mdy;
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




