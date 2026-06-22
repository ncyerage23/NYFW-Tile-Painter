/*
 * PALETTE MODULE
 *
 * This is gonna be the annoying one, I know. Basically, I just need a little module,
 * like the tile one, that allows the user to pick what color they're painting. I'll
 * add a little highlight thing around the specific one. 
 *
 */

#include "tpaint.h"

uint16_t current_color = 0x0000;
int cur_col_idx = 0;

int blen = 10;
int col_size = 50; 


typedef struct {
	NYFW_Rect palette_rect;		// the whole module
	NYFW_Rect border_rects[4];
	NYFW_Rect color_rects[16];	// for the color picker tiles
	
	NYFW_Canvas scr;		// maybe make this a global? Idk. 
} PaletteModule;

PaletteModule pmod;


int palette_mod_init(NYFW_Canvas scr)
{
	pmod.scr = scr;

	/* ----- SCREEN DIMENSIONS ----- */
	int sw = scr.width,	sh = scr.height;
	int centerx = sw/2,	centery = sh/2;
	int quarterx = sw/8;

	/* ----- PALETTE RECT ----- */
	int rl_2 = col_size * 2;
	int rl = col_size * 4;
	int rx = quarterx - rl_2,	ry = centery - rl_2;
	pmod.palette_rect = (NYFW_Rect){ rx, ry, rl, rl };

	/* ----- BORDER RECTS ----- */
	int cvx = rx-blen,	cvy = ry-blen;
	int cvl = rl + 2 * blen;

	pmod.border_rects[0] = (NYFW_Rect){ cvx, cvy, cvl, blen };		// top
	pmod.border_rects[1] = (NYFW_Rect){ cvx, cvy+blen+rl, cvl, blen };	// bottom
	pmod.border_rects[2] = (NYFW_Rect){ cvx, cvy+blen, blen, rl };		// left
	pmod.border_rects[3] = (NYFW_Rect){ cvx+blen+rl, cvy+blen, blen, rl };	// right

	/* ----- COLOR RECTS ----- */
	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 4; i++)
			pmod.color_rects[j * 4 + i] = (NYFW_Rect){ rx + i * col_size, ry + j * col_size, col_size, col_size };

	return 1;
}


void palette_mod_draw()
{
	for (int i = 0; i < 4; i++)
		nyfw_canvasFill(pmod.scr, LGREEN, &pmod.border_rects[i]);

	for (int i = 0; i < 16; i++)
		nyfw_canvasFill(pmod.scr, NYFW_Palette[i], &pmod.color_rects[i]);


	// highlight current color
	NYFW_Rect cur_rect = pmod.color_rects[cur_col_idx];
	int x = cur_rect.x,	y = cur_rect.y;
	int w = cur_rect.w,	h = cur_rect.h;

	for (int i = x; i < x+w; i++) {
		nyfw_canvSetPixel(pmod.scr, i, y, ~current_color);
		nyfw_canvSetPixel(pmod.scr, i, y+h-1, ~current_color);
	}

	for (int j = y+1; j < y+h-1; j++) {
		nyfw_canvSetPixel(pmod.scr, x, j, ~current_color);
		nyfw_canvSetPixel(pmod.scr, x+w-1, j, ~current_color);
	}

}


void palette_check_input(int x, int y)
{
	if (!IN_RECT(pmod.palette_rect, x, y)) return;

	for (int i = 0; i < 16; i++) {
		if (IN_RECT(pmod.color_rects[i], x, y)) {
			current_color = NYFW_Palette[i];
			cur_col_idx = i;
			break;
		}
	}

}










