/* 
 * FILE READING FOR TPAINT
 *
 * Basically, just reading the arguments to see if there's a file of that name
 * in the "gallery" folder -- will change this later when I use tpaint anywhere --
 * and: if so, load that tile and use it as the tile being edited. If not, 
 * create a new canvas, and save it as the name in the arguments. If nothing, idk
 * shut down. 
 *
 * TODO: use the filename as the title thing. Like at the top of the screen? Or the bottom, if/when I do commands. Like NVIM.
 *
 *
 * Gonna try remaking this whole thing, because I'm stupid. Here's the plan: just use loadNYMG to check, don't use all this bs. 
 * Yeah. 
 *
 */


#include "tpaint.h"
#include <stdlib.h>
#include <string.h>

char file_path[256];
char filename[256];


int init_file(const char* arg)
{
	if (strlen(arg) == 0) {
		strcat(filename, "default.nymg");
		strcat(file_path, "gallery/");
		strcat(file_path, filename);

		uint16_t* pixels = malloc(128);
		tile = nyfw_canvas(pixels, 8, 8, 8);
		return 1;
	}
	
	strcat(filename, arg);
	strcat(file_path, "gallery/");
	
	const char* ext = strrchr(filename, '.');
	if (ext != NULL && strcmp(ext, ".nymg") != 0) {
		printf("error: incorrect file extension\n");
		return 0;
	}
	else if (ext == NULL) {
		strcat(filename, ".nymg");
	}

	strcat(file_path, filename);

	if (!nyfw_loadNYMG(&tile, file_path)) {
		uint16_t* pixels = malloc(128);
		tile = nyfw_canvas(pixels, 8, 8, 8);
	}

	return 1;
}


void free_tile()
{
	free(tile.pixels);
}


void save_file()
{
	nyfw_saveNYMG(tile, file_path);
	free_tile();
}



