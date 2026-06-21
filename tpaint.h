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


/* ----- TILE MODULE ----- */
int tile_mod_init(NYFW_Canvas scr);
void tile_mod_draw();
void tile_check_input(int x, int y);
NYFW_Canvas get_tile();



#endif
