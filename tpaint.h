/*
 * TILE PAINTER
 *
 * A program for painting little 8x8 tiles for games. Saved to my .nymg format. 
 * Idk what else to put here, but yeah. This is the header file. 
 *
 */

#ifndef TPAINT_H
#define TPAINT_H

#include "NYFW/nyfw.h"
#include <stdio.h>

#define IN_RECT(r, x, y)	((r).x <= (x) && (r).y <= (y) && (x) < (r).x + (r).w && (y) < (r).y + (r).h) ? 1 : 0


/* ----- GLOBALS ----- */
extern NYFW_Canvas tile;
extern char file_path[256];
extern uint16_t current_color;


/* ----- TILE MODULE ----- */
int tile_mod_init(NYFW_Canvas scr);
void tile_mod_draw();
void tile_check_input(int x, int y);
NYFW_Canvas get_tile();


/* ----- FILE MODULE ----- */
int init_file(const char* arg);
void save_file();
void free_tile();


/* ----- PALETTE MODULE ----- */
int palette_mod_init(NYFW_Canvas scr);
void palette_mod_draw();
void palette_check_input(int x, int y);

#endif
