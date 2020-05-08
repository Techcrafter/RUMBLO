#include <fileioc.h>
#include <fontlibc.h>
#include "gfx/gfx.h"
#include <graphx.h>
#include <keypadc.h>
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
#define GRAY				3
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

int player;
int playerX;
int playerY;
int playerGrounded;

int player1CharacterSelection;
int player1X;
int player1Y;
int player1Flipped;
int player1MoveSpeed;
int player1FallSpeed;
int player1JumpSpeed;
int player1JumpHeight;
int player1MoveAnimation;
int player1MoveAnimationCount;
int player1Jumping;
int player1Grounded;
int player1ShieldActive;
int player1Lifes;

int player2IsAi;
int player2CharacterSelection;
int player2X;
int player2Y;
int player2Flipped;
int player2MoveSpeed;
int player2FallSpeed;
int player2JumpSpeed;
int player2JumpHeight;
int player2MoveAnimation;
int player2MoveAnimationCount;
int player2Jumping;
int player2Grounded;
int player2ShieldActive;
int player2Lifes;

sk_key_t key;

gfx_sprite_t *player1Character;
gfx_sprite_t *player1CharacterMoving1;
gfx_sprite_t *player1CharacterMoving2;
gfx_sprite_t *player1CharacterJumping;
gfx_sprite_t *player1CharacterFlipped;
gfx_sprite_t *player1CharacterMoving1Flipped;
gfx_sprite_t *player1CharacterMoving2Flipped;
gfx_sprite_t *player1CharacterJumpingFlipped;

gfx_sprite_t *player2Character;
gfx_sprite_t *player2CharacterMoving1;
gfx_sprite_t *player2CharacterMoving2;
gfx_sprite_t *player2CharacterJumping;
gfx_sprite_t *player2CharacterFlipped;
gfx_sprite_t *player2CharacterMoving1Flipped;
gfx_sprite_t *player2CharacterMoving2Flipped;
gfx_sprite_t *player2CharacterJumpingFlipped;

gfx_tilemap_t tilemap;

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
	player1Character = gfx_MallocSprite(32, 48);
	player1CharacterMoving1 = gfx_MallocSprite(32, 48);
	player1CharacterMoving2 = gfx_MallocSprite(32, 48);
	player1CharacterJumping = gfx_MallocSprite(32, 48);
	player1CharacterFlipped = gfx_MallocSprite(32, 48);
	player1CharacterMoving1Flipped = gfx_MallocSprite(32, 48);
	player1CharacterMoving2Flipped = gfx_MallocSprite(32, 48);
	player1CharacterJumpingFlipped = gfx_MallocSprite(32, 48);
	
	player2Character = gfx_MallocSprite(32, 48);
	player2CharacterMoving1 = gfx_MallocSprite(32, 48);
	player2CharacterMoving2 = gfx_MallocSprite(32, 48);
	player2CharacterJumping = gfx_MallocSprite(32, 48);
	player2CharacterFlipped = gfx_MallocSprite(32, 48);
	player2CharacterMoving1Flipped = gfx_MallocSprite(32, 48);
	player2CharacterMoving2Flipped = gfx_MallocSprite(32, 48);
	player2CharacterJumpingFlipped = gfx_MallocSprite(32, 48);
	
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
	gfx_PrintStringXY("Press [2nd] to start...", 78, 200);
	gfx_PrintStringXY(version, 296, 232);
	gfx_SwapDraw();
	while(os_GetCSC() != sk_2nd);
	
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
		if(key == sk_2nd)
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
		gfx_SetTextFGColor(WHITE);
		gfx_SetTextBGColor(BLACK);
		gfx_SetTextScale(2, 2);
		gfx_PrintStringXY("Select a map:", 62, 10);
		
		key = os_GetCSC();
		if(key == sk_2nd)
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
		gfx_SetColor(GRAY);
		gfx_FillRectangle(0, 70, 320, 105);
		gfx_SetTextFGColor(WHITE);
		gfx_SetTextBGColor(GRAY);
		gfx_SetTextScale(2, 2);
		gfx_PrintStringXY("Select a character:", 10, 76);
		gfx_TransparentSprite(characterHighlighter, 138, 100);
		
		key = os_GetCSC();
		if(key == sk_2nd)
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
				gfx_TransparentSprite(character1, 77, 108);
				gfx_TransparentSprite(character0, 146, 108);
				gfx_TransparentSprite(character1, 215, 108);
				break;
			case 1:
				gfx_TransparentSprite(character0, 77, 108);
				gfx_TransparentSprite(character1, 146, 108);
				gfx_TransparentSprite(character0, 215, 108);
				break;
		}
		
		gfx_SwapDraw();
	}
	player1CharacterSelection = selection;
	
	goto prepareFight;
	
	//---------------------------------------------------------------
	
	//prepareFight
	prepareFight:
	
	fighting = 1;
	player1Flipped = 0;
	player1MoveAnimation = 0;
	player1Jumping = 0;
	player1Grounded = 1;
	player1ShieldActive = 0;
	player1Lifes = 3;
	player2Flipped = 1;
	player2MoveAnimation = 0;
	player2Jumping = 0;
	player2Grounded = 1;
	player2ShieldActive = 0;
	player2Lifes = 3;
	switch(map)
	{
		case 0:
			player1X = 80;
			player1Y = 132;
			if(players == 1)
			{
				player2IsAi = 1;
			}
			else if(players == 2)
			{
				player2IsAi = 0;
			}
			player2X = 220;
			player2Y = 132;
			break;
		case 1:
			break;
	}
	
	switch(player1CharacterSelection)
	{
		case 0:
			player1Character = character0;
			player1CharacterMoving1 = character0Moving1;
			player1CharacterMoving2 = character0Moving2;
			player1CharacterJumping = character0Jumping;
			gfx_FlipSpriteY(character0, player1CharacterFlipped);
			gfx_FlipSpriteY(character0Moving1, player1CharacterMoving1Flipped);
			gfx_FlipSpriteY(character0Moving2, player1CharacterMoving2Flipped);
			gfx_FlipSpriteY(character0Jumping, player1CharacterJumpingFlipped);
			player1MoveSpeed = 4;
			player1FallSpeed = 4;
			player1JumpSpeed = 8;
			player1JumpHeight = 10;
			break;
		case 1:
			player1Character = character1;
			player1CharacterMoving1 = character1Moving1;
			player1CharacterMoving2 = character1Moving2;
			player1CharacterJumping = character1Jumping;
			gfx_FlipSpriteY(character1, player1CharacterFlipped);
			gfx_FlipSpriteY(character1Moving1, player1CharacterMoving1Flipped);
			gfx_FlipSpriteY(character1Moving2, player1CharacterMoving2Flipped);
			gfx_FlipSpriteY(character1Jumping, player1CharacterJumpingFlipped);
			player1MoveSpeed = 8;
			player1FallSpeed = 4;
			player1JumpSpeed = 4;
			player1JumpHeight = 12;
			break;
	}
	switch(player2CharacterSelection)
	{
		case 0:
			player2Character = character0;
			player2CharacterMoving1 = character0Moving1;
			player2CharacterMoving2 = character0Moving2;
			player2CharacterJumping = character0Jumping;
			gfx_FlipSpriteY(character0, player2CharacterFlipped);
			gfx_FlipSpriteY(character0Moving1, player2CharacterMoving1Flipped);
			gfx_FlipSpriteY(character0Moving2, player2CharacterMoving2Flipped);
			gfx_FlipSpriteY(character0Jumping, player2CharacterJumpingFlipped);
			player2MoveSpeed = 4;
			player2FallSpeed = 4;
			player2JumpSpeed = 8;
			player2JumpHeight = 10;
			break;
		case 1:
			player2Character = character1;
			player2CharacterMoving1 = character1Moving1;
			player2CharacterMoving2 = character1Moving2;
			player2CharacterJumping = character1Jumping;
			gfx_FlipSpriteY(character1, player2CharacterFlipped);
			gfx_FlipSpriteY(character1Moving1, player2CharacterMoving1Flipped);
			gfx_FlipSpriteY(character1Moving2, player2CharacterMoving2Flipped);
			gfx_FlipSpriteY(character1Jumping, player2CharacterJumpingFlipped);
			player2MoveSpeed = 8;
			player2FallSpeed = 4;
			player2JumpSpeed = 4;
			player2JumpHeight = 12;
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
	
	gfx_TransparentSprite(statusUi, 128, 208);
	gfx_SetTextFGColor(BLACK);
	gfx_SetTextBGColor(gfx_GetPixel(150, 224));
	
	while(fighting)
	{
		gfx_Tilemap(&tilemap, getXBlock(mapXBlock), getYBlock(mapYBlock));
		
		key = os_GetCSC();
		if(key == sk_2nd)
		{
			if(player1Grounded == 1)
			{
				player1Jumping = player1JumpHeight;
			}
		}
		if(key == sk_Alpha)
		{
			//shoot
		}
		if(key == sk_Clear)
		{
			goto pause;
		}
		
		kb_Scan();
        key = kb_Data[7];
		if(key & kb_Up)
		{
			
		}
		if(key & kb_Down)
		{
			player1Jumping = 0;
			if(player1Grounded == 1)
			{
				player1ShieldActive = 1;
			}
		}
		else
		{
			player1ShieldActive = 0;
		}
		if(key & kb_Left)
		{
			player1X -= player1MoveSpeed;
			if(player1Flipped == 0)
			{
				player1Flipped = 1;
				player1MoveAnimation = 0;
				player1MoveAnimationCount = 0;
			}
			player1MoveAnimationCount++;
			if(player1MoveAnimationCount == 3)
			{
				if(player1MoveAnimation != 3)
				{
					player1MoveAnimation++;
				}
				else
				{
					player1MoveAnimation = 0;
				}
				player1MoveAnimationCount = 0;
			}
		}
		if(key & kb_Right)
		{
			player1X += player1MoveSpeed;
			if(player1Flipped == 1)
			{
				player1Flipped = 0;
				player1MoveAnimation = 0;
				player1MoveAnimationCount = 0;
			}
			player1MoveAnimationCount++;
			if(player1MoveAnimationCount == 3)
			{
				if(player1MoveAnimation != 3)
				{
					player1MoveAnimation++;
				}
				else
				{
					player1MoveAnimation = 0;
				}
				player1MoveAnimationCount = 0;
			}
		}
		
		for(player = 1; player < 3; ++player)
		{
			switch(player)
			{
				case 1:
					playerX = player1X;
					playerY = player1Y;
					break;
				case 2:
					playerX = player2X;
					playerY = player2Y;
					break;
			}
			
			if(map == 0)
			{
				if(playerX >= 24 && playerX <= 264 && playerY == 132)
				{
					playerGrounded = 1;
				}
				else if(playerX >= 72 && playerX <= 216 && playerY == 64)
				{
					playerGrounded = 1;
				}
				else if(playerX >= -8 && playerX <= 40 && playerY == 40)
				{
					playerGrounded = 1;
				}
				else if(playerX >= 248 && playerX <= 296 && playerY == 40)
				{
					playerGrounded = 1;
				}
				else
				{
					playerGrounded = 0;
				}
			}
			
			switch(player)
			{
				case 1:
					player1Grounded = playerGrounded;
					break;
				case 2:
					player2Grounded = playerGrounded;
					break;
			}
		}
		
		if(player1Jumping > 0)
		{
			player1Jumping--;
			player1Y -= player1JumpSpeed;
		}
		else if(player1Grounded == 0)
		{
			player1Y += player1FallSpeed;
		}
		
		if(player2Jumping > 0)
		{
			player2Jumping--;
			player2Y -= player2JumpSpeed;
		}
		else if(player2Grounded == 0)
		{
			player2Y += player2FallSpeed;
		}
		
		if(player1Flipped == 0 && player1Grounded == 0)
		{
			gfx_TransparentSprite(player1CharacterJumping, player1X, player1Y);
			player1MoveAnimation = 0;
			player1MoveAnimationCount = 0;
		}
		else if(player1Flipped == 1 && player1Grounded == 0)
		{
			gfx_TransparentSprite(player1CharacterJumpingFlipped, player1X, player1Y);
			player1MoveAnimation = 0;
			player1MoveAnimationCount = 0;
		}
		else if(player1Flipped == 0 && player1MoveAnimation == 0)
		{
			gfx_TransparentSprite(player1Character, player1X, player1Y);
		}
		else if(player1Flipped == 1 && player1MoveAnimation == 0)
		{
			gfx_TransparentSprite(player1CharacterFlipped, player1X, player1Y);
		}
		else if(player1Flipped == 0 && player1MoveAnimation == 1)
		{
			gfx_TransparentSprite(player1CharacterMoving1, player1X, player1Y);
		}
		else if(player1Flipped == 1 && player1MoveAnimation == 1)
		{
			gfx_TransparentSprite(player1CharacterMoving1Flipped, player1X, player1Y);
		}
		else if(player1Flipped == 0 && player1MoveAnimation == 2)
		{
			gfx_TransparentSprite(player1CharacterMoving2, player1X, player1Y);
		}
		else if(player1Flipped == 1 && player1MoveAnimation == 2)
		{
			gfx_TransparentSprite(player1CharacterMoving2Flipped, player1X, player1Y);
		}
		else if(player1Flipped == 0 && player1MoveAnimation == 3)
		{
			gfx_TransparentSprite(player1CharacterMoving1, player1X, player1Y);
		}
		else if(player1Flipped == 1 && player1MoveAnimation == 3)
		{
			gfx_TransparentSprite(player1CharacterMoving1Flipped, player1X, player1Y);
		}
		
		
		if(player2Flipped == 0 && player2Grounded == 0)
		{
			gfx_TransparentSprite(player2CharacterJumping, player2X, player2Y);
			player2MoveAnimation = 0;
			player2MoveAnimationCount = 0;
		}
		else if(player2Flipped == 1 && player2Grounded == 0)
		{
			gfx_TransparentSprite(player2CharacterJumpingFlipped, player2X, player2Y);
			player2MoveAnimation = 0;
			player2MoveAnimationCount = 0;
		}
		else if(player2Flipped == 0 && player2MoveAnimation == 0)
		{
			gfx_TransparentSprite(player2Character, player2X, player2Y);
		}
		else if(player2Flipped == 1 && player2MoveAnimation == 0)
		{
			gfx_TransparentSprite(player2CharacterFlipped, player2X, player2Y);
		}
		else if(player2Flipped == 0 && player2MoveAnimation == 1)
		{
			gfx_TransparentSprite(player2CharacterMoving1, player2X, player2Y);
		}
		else if(player2Flipped == 1 && player2MoveAnimation == 1)
		{
			gfx_TransparentSprite(player2CharacterMoving1Flipped, player2X, player2Y);
		}
		else if(player2Flipped == 0 && player2MoveAnimation == 2)
		{
			gfx_TransparentSprite(player2CharacterMoving2, player2X, player2Y);
		}
		else if(player2Flipped == 1 && player2MoveAnimation == 2)
		{
			gfx_TransparentSprite(player2CharacterMoving2Flipped, player2X, player2Y);
		}
		else if(player2Flipped == 0 && player2MoveAnimation == 3)
		{
			gfx_TransparentSprite(player2CharacterMoving1, player2X, player2Y);
		}
		else if(player2Flipped == 1 && player2MoveAnimation == 3)
		{
			gfx_TransparentSprite(player2CharacterMoving1Flipped, player2X, player2Y);
		}
		
		gfx_TransparentSprite(statusUi, 128, 208);
		gfx_SetTextXY(10, 10);
		gfx_SetTextScale(2, 2);
		gfx_PrintInt(player1X, 3);
		gfx_PrintString(" ");
		gfx_PrintInt(player1Y, 3);
		
		gfx_SwapDraw();
	}
	
	goto error;
	
	//---------------------------------------------------------------
	
	//pause
	pause:
	
	gfx_Tilemap(&tilemap, getXBlock(mapXBlock), getYBlock(mapYBlock));
	gfx_SetColor(GRAY);
	gfx_FillRectangle(0, 70, 320, 105);
	gfx_SetTextFGColor(WHITE);
	gfx_SetTextBGColor(GRAY);
	gfx_SetTextScale(2, 2);
	gfx_PrintStringXY("Pause menu", 80, 90);
	gfx_SetTextScale(1, 1);
	gfx_PrintStringXY("Press [clear] to continue...", 55, 125);
	gfx_PrintStringXY("Press [del] to give up...", 62, 145);
	gfx_SwapDraw();
	
	while(1)
	{
		key = os_GetCSC();
		if(key == sk_Clear)
		{
			goto fight;
		}
		else if(key == sk_Del)
		{
			goto gameOver;
		}
	}
	
	//---------------------------------------------------------------
	
	//gameOver
	gameOver:
	
	gfx_Tilemap(&tilemap, getXBlock(4), getYBlock(0));
	gfx_SetTextFGColor(WHITE);
	gfx_SetTextBGColor(gfx_GetPixel(0, 0));
	gfx_SetTextScale(1, 1);
	gfx_PrintStringXY("Press [2nd] to go to the main menu...", 15, 218);
	gfx_SwapDraw();
	while(os_GetCSC() != sk_2nd);
	goto mainMenu;
	
	
	//---------------------------------------------------------------
	
	//error
	error:
	
	gfx_FillScreen(RED);
	gfx_SetTextFGColor(WHITE);
	gfx_SetTextBGColor(RED);
	gfx_SetTextScale(1, 1);
	gfx_PrintStringXY("An error has occurred!", 0, 0);
	gfx_PrintStringXY("Press [2nd] to go to the main menu...", 0, 10);
	gfx_SwapDraw();
	while(os_GetCSC() != sk_2nd);
	goto mainMenu;
	
	//---------------------------------------------------------------
	
	//quit game
	quitGame:
	
	gfx_End();
	return 0;
}