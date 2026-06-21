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

#include "tpaint.h"
#include <unistd.h>


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

	NYFW_Canvas out = get_tile();
	nyfw_saveNYMG(out, "gallery/poo.nymg");

	shutdown();
	return 0;
}




