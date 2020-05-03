#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <graphx.h>

#include <fileioc.h>
#include <fontlibc.h>

#include "gfx/gfx.h"

extern unsigned char tilemap_map[];

#define TILE_WIDTH          16
#define TILE_HEIGHT         16

#define TILEMAP_WIDTH       32
#define TILEMAP_HEIGHT      25

#define TILEMAP_DRAW_WIDTH  20
#define TILEMAP_DRAW_HEIGHT 14

#define Y_OFFSET            0
#define X_OFFSET            0

int main(void)
{
	sk_key_t key;
	
	fontlib_font_t *Calvetica;
	
    unsigned int x_offset = 0;
    unsigned int y_offset = 0;
    gfx_tilemap_t tilemap;
	
	tilemap.map         = tilemap_map;
    tilemap.tiles       = tileset_tiles;
    tilemap.type_width  = gfx_tile_16_pixel;
    tilemap.type_height = gfx_tile_16_pixel;
    tilemap.tile_height = TILE_HEIGHT;
    tilemap.tile_width  = TILE_WIDTH;
    tilemap.draw_height = TILEMAP_DRAW_HEIGHT;
    tilemap.draw_width  = TILEMAP_DRAW_WIDTH;
    tilemap.height      = TILEMAP_HEIGHT;
    tilemap.width       = TILEMAP_WIDTH;
    tilemap.y_loc       = Y_OFFSET;
    tilemap.x_loc       = X_OFFSET;
	
	gfx_Begin();
	
	gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetColor(0);
    gfx_SetTextFGColor(1);
    gfx_SetTextBGColor(0);
	
	gfx_SetDrawBuffer();
	
	gfx_SetMonospaceFont(8);
	
    while (!os_GetCSC())
	{
		gfx_FillRectangle(0, 224, 320, 16);
		gfx_PrintStringXY("X:", 4, 232);
        gfx_PrintUInt(x_offset, 4);
        gfx_PrintString("      Y:");
        gfx_PrintUInt(y_offset, 4);
		gfx_Tilemap(&tilemap, x_offset, y_offset);
		gfx_SwapDraw();
		delay(1000);
		y_offset += TILE_HEIGHT;
		x_offset += TILE_WIDTH;
		gfx_Tilemap(&tilemap, x_offset, y_offset);
	}
	
	gfx_End();
	
	return 0;
}
