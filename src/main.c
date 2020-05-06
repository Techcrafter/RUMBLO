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

#define BLACK				1
#define GRAY				13
#define RED					gfx_red
#define TRANSPARENT			0
#define WHITE				2

char version[] = "1.0";

int selection;
int selecting;

int players;
int map;
int mapXBlock;
int mapYBlock;
int fighting;

int player1Character;
int player1X;
int player1Y;
int player1Lifes;

int player2IsAi;
int player2Character;
int player2X;
int player2Y;
int player2Lifes;

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
	//initialization
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
	gfx_SetTransparentColor(TRANSPARENT);
    gfx_SetColor(WHITE);
	
	gfx_SetDrawBuffer();
	
	gfx_SetMonospaceFont(8);
	gfx_SetTextFGColor(BLACK);
    gfx_SetTextBGColor(WHITE);
	
	gfx_Tilemap(&tilemap, getXBlock(0), getYBlock(0));
	gfx_PrintStringXY(version, 296, 232);
	gfx_SwapDraw();
	
	delay(2000);
	
	//---------------------------------------------------------------
	
	//start screen
	startScreen:
	
	gfx_Tilemap(&tilemap, getXBlock(0), getYBlock(0));
	gfx_SetTextFGColor(BLACK);
    gfx_SetTextBGColor(WHITE);
	gfx_SetTextScale(1, 1);
	gfx_PrintStringXY("Press [enter] to start...", 65, 200);
	gfx_PrintStringXY(version, 296, 232);
	gfx_SwapDraw();
	while(os_GetCSC() != sk_Enter);
	
	//---------------------------------------------------------------
	
	//main menu
	mainMenu:
	
	selection = 0;
	selecting = 1;
	while(selecting)
	{
		gfx_Tilemap(&tilemap, getXBlock(1), getYBlock(0));
		gfx_SetTextBGColor(WHITE);
		gfx_SetTextScale(1, 1);
		
		key = os_GetCSC();
		if(key == sk_Enter)
		{
			selecting = 0;
		}
		else if(key == sk_Clear)
		{
			goto startScreen;
		}
		else if(key == sk_Up && selection != 0)
		{
			selection--;
		}
		else if(key == sk_Down && selection != 1)
		{
			selection++;
		}
		
		if(selection == 0)
		{
			gfx_SetTextFGColor(RED);
		}
		else
		{
			gfx_SetTextFGColor(BLACK);
		}
		gfx_PrintStringXY("Singleplayer", 112, 68);
		if(selection == 1)
		{
			gfx_SetTextFGColor(RED);
		}
		else
		{
			gfx_SetTextFGColor(BLACK);
		}
		gfx_PrintStringXY("Quit game", 124, 100);
		gfx_SetTextFGColor(BLACK);
		gfx_PrintStringXY("Star it on GitHub!", 90, 220);
		
		gfx_SwapDraw();
	}
	
	switch(selection)
	{
		case 0:
			players = 1;
			goto mapSelector;
			break;
		case 1:
			goto quitGame;
			break;
	}
	
	goto error;
	
	//---------------------------------------------------------------
	
	//map selector
	mapSelector:
	
	selection = 0;
	selecting = 1;
	while(selecting)
	{
		gfx_Tilemap(&tilemap, getXBlock(2), getYBlock(0));
		gfx_SetColor(GRAY);
		gfx_SetTextFGColor(WHITE);
		gfx_SetTextBGColor(BLACK);
		gfx_SetTextScale(2, 2);
		gfx_PrintStringXY("Select a map:", 62, 10);
		
		key = os_GetCSC();
		if(key == sk_Enter)
		{
			selecting = 0;
		}
		else if(key == sk_Clear)
		{
			goto mainMenu;
		}
		else if(key == sk_Left && selection != 0)
		{
			selection--;
		}
		else if(key == sk_Right && selection != 1)
		{
			selection++;
		}
		
		switch(selection)
		{
			case 0:
				gfx_TransparentSprite(mapHighlighter, 32, 64);
				break;
			case 1:
				gfx_TransparentSprite(mapHighlighter, 96, 64);
				break;
		}
		gfx_SwapDraw();
	}
	map = selection;
	switch(map)
	{
		case 0:
			mapXBlock = 3;
			mapYBlock = 0;
			break;
		case 1:
			break;
	}
	
	goto characterSelector;
	
	//---------------------------------------------------------------
	
	//character selector
	characterSelector:
	
	selection = 0;
	selecting = 1;
	
	while(selecting)
	{
		switch(map)
		{
			case 0:
				gfx_Tilemap(&tilemap, getXBlock(mapXBlock), getYBlock(mapYBlock));
				break;
			case 1:
				goto error;
				break;
		}
		gfx_FillRectangle(0, 70, 320, 105);
		gfx_SetTextFGColor(WHITE);
		gfx_SetTextBGColor(GRAY);
		gfx_SetTextScale(2, 2);
		gfx_PrintStringXY("Select a character:", 10, 76);
		gfx_TransparentSprite(characterHighlighter, 138, 100);
		
		key = os_GetCSC();
		if(key == sk_Enter)
		{
			selecting = 0;
		}
		else if(key == sk_Clear)
		{
			goto mapSelector;
		}
		else if(key == sk_Left && selection != 0)
		{
			selection--;
		}
		else if(key == sk_Left && selection == 0)
		{
			selection = 1;
		}
		else if(key == sk_Right && selection != 1)
		{
			selection++;
		}
		else if(key == sk_Right && selection == 1)
		{
			selection = 0;
		}
		
		switch(selection)
		{
			case 0:
				gfx_TransparentSprite(character1, 69, 100);
				gfx_TransparentSprite(character0, 138, 100);
				gfx_TransparentSprite(character1, 207, 100);
				break;
			case 1:
				gfx_TransparentSprite(character0, 69, 100);
				gfx_TransparentSprite(character1, 138, 100);
				gfx_TransparentSprite(character0, 207, 100);
				break;
		}
		gfx_SwapDraw();
	}
	player1Character = selection;
	
	goto prepareFight;
	
	//---------------------------------------------------------------
	
	//prepareFight
	prepareFight:
	
	fighting = 1;
	switch(map)
	{
		case 0:
			player1X = 100;
			player1Y = 100;
			player1Lifes = 3;
			if(players == 1)
			{
				player2IsAi = 1;
			}
			else if(players == 2)
			{
				player2IsAi = 0;
			}
			player2X = 200;
			player2Y = 200;
			player2Lifes = 3;
			break;
		case 1:
			break;
	}
	
	gfx_Tilemap(&tilemap, getXBlock(mapXBlock), getYBlock(mapYBlock));
	gfx_TransparentSprite(counter3, 136, 104);
	gfx_SwapDraw();
	delay(1000);
	gfx_Tilemap(&tilemap, getXBlock(mapXBlock), getYBlock(mapYBlock));
	gfx_TransparentSprite(counter2, 136, 104);
	gfx_SwapDraw();
	delay(1000);
	gfx_Tilemap(&tilemap, getXBlock(mapXBlock), getYBlock(mapYBlock));
	gfx_TransparentSprite(counter1, 136, 104);
	gfx_SwapDraw();
	delay(1000);
	gfx_Tilemap(&tilemap, getXBlock(mapXBlock), getYBlock(mapYBlock));
	gfx_TransparentSprite(counterFight, 136, 104);
	gfx_SwapDraw();
	delay(1000);
	
	goto fight;
	
	//---------------------------------------------------------------
	
	//fight
	fight:
	
	while(fighting)
	{
		gfx_Tilemap(&tilemap, getXBlock(mapXBlock), getYBlock(mapYBlock));
		
		key = os_GetCSC();
		if(key == sk_Clear)
		{
			fighting = 0;
		}
		else if(key == sk_Up)
		{
			player1Y--;
		}
		else if(key == sk_Down)
		{
			player1Y++;
		}
		else if(key == sk_Left)
		{
			player1X--;
		}
		else if(key == sk_Right)
		{
			player1X++;
		}
		
		switch(player1Character)
		{
			case 0:
				gfx_TransparentSprite(character0, player1X, player1Y);
				break;
			case 1:
				gfx_TransparentSprite(character1, player1X, player1Y);
				break;
		}
		
		gfx_SwapDraw();
	}
	
	goto error;
	
	//---------------------------------------------------------------
	
	//error
	error:
	
	gfx_FillScreen(RED);
	gfx_SetTextFGColor(WHITE);
	gfx_SetTextBGColor(RED);
	gfx_SetTextScale(1, 1);
	gfx_PrintStringXY("An error has occurred!", 0, 0);
	gfx_PrintStringXY("Press [enter] to go to the main menu...", 0, 10);
	gfx_SwapDraw();
	while(os_GetCSC() != sk_Enter);
	goto mainMenu;
	
	//---------------------------------------------------------------
	
	//quit game
	quitGame:
	
	gfx_End();
	return 0;
}