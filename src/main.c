#include <fileioc.h>
#include <fontlibc.h>
#include "gfx/gfx.h"
#include <graphx.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tice.h>

extern unsigned char tilemap_map[];

#define TILE_WIDTH          16
#define TILE_HEIGHT         16

#define TILEMAP_WIDTH       160
#define TILEMAP_HEIGHT      120

#define TILEMAP_DRAW_WIDTH  20
#define TILEMAP_DRAW_HEIGHT 15

#define Y_OFFSET            0
#define X_OFFSET            0

#define BLACK				7
#define WHITE				0

int getXBlock(int xBlock)
{
	return TILE_WIDTH * TILEMAP_DRAW_WIDTH * xBlock;
}

int getYBlock(int yBlock)
{
	return TILE_HEIGHT * TILEMAP_DRAW_HEIGHT * yBlock;
}

int main(void)
{
	sk_key_t key;
	
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
    gfx_SetColor(WHITE);
    gfx_SetTextFGColor(BLACK);
    gfx_SetTextBGColor(WHITE);
	
	gfx_SetDrawBuffer();
	
	gfx_SetMonospaceFont(8);
	
	gfx_Tilemap(&tilemap, getXBlock(0), getYBlock(0));
	gfx_SwapDraw();
	
	delay(3000);
	
	gfx_Tilemap(&tilemap, getXBlock(0), getYBlock(0));
	gfx_PrintStringXY("Press [enter] to begin...", 60, 200);
	gfx_SwapDraw();
	while(os_GetCSC() != sk_Enter);
	
	while(1)
	{
		key = os_GetCSC();
		
		switch(key)
		{
			
		}
	}
	
	gfx_Tilemap(&tilemap, getXBlock(1), getYBlock(0));
	gfx_SetTextFGColor(BLACK);
	gfx_PrintStringXY("Quit game", 110, 100);
	gfx_SwapDraw();
	
	while(os_GetCSC() != sk_Enter);
	
	gfx_End();
	return 0;
}
