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
int testMode = 0;

int selection;
int selecting;

int players;
int map;
int mapXBlock;
int mapYBlock;

int character0MoveSpeed;
int character0FallSpeed;
int character0JumpSpeed;
int character0JumpHeight;

int character0WeaponSpeed;
int character0WeaponKnockback;
int character0WeaponKnockbackStrength;

int character1MoveSpeed;
int character1FallSpeed;
int character1JumpSpeed;
int character1JumpHeight;

int character1WeaponSpeed;
int character1WeaponKnockback;
int character1WeaponKnockbackStrength;

int player;
int playerX;
int playerY;
int playerGrounded;
int playerDeath;

int player1SpawnX;
int player1SpawnY;
int player2SpawnX;
int player2SpawnY;

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
int player1XKnockback;
int player1Lifes;

int player1WeaponSpeed;
int player1WeaponKnockback;
int player1WeaponKnockbackStrength;
int player1Weapon1;
int player1Weapon1X;
int player1Weapon1Y;
int player1Weapon1Flipped;
int player1Weapon2;
int player1Weapon2X;
int player1Weapon2Y;
int player1Weapon2Flipped;
int player1Weapon3;
int player1Weapon3X;
int player1Weapon3Y;
int player1Weapon3Flipped;

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
int player2XKnockback;
int player2Lifes;

int player2WeaponSpeed;
int player2WeaponKnockback;
int player2WeaponKnockbackStrength;
int player2Weapon1;
int player2Weapon1X;
int player2Weapon1Y;
int player2Weapon1Flipped;
int player2Weapon2;
int player2Weapon2X;
int player2Weapon2Y;
int player2Weapon2Flipped;
int player2Weapon3;
int player2Weapon3X;
int player2Weapon3Y;
int player2Weapon3Flipped;

sk_key_t key;

gfx_sprite_t *player1Character;
gfx_sprite_t *player1CharacterMoving1;
gfx_sprite_t *player1CharacterMoving2;
gfx_sprite_t *player1CharacterNotGrounded;
gfx_sprite_t *player1CharacterFlipped;
gfx_sprite_t *player1CharacterMoving1Flipped;
gfx_sprite_t *player1CharacterMoving2Flipped;
gfx_sprite_t *player1CharacterNotGroundedFlipped;

gfx_sprite_t *player1Weapon;
gfx_sprite_t *player1WeaponFlipped;

gfx_sprite_t *player2Character;
gfx_sprite_t *player2CharacterMoving1;
gfx_sprite_t *player2CharacterMoving2;
gfx_sprite_t *player2CharacterNotGrounded;
gfx_sprite_t *player2CharacterFlipped;
gfx_sprite_t *player2CharacterMoving1Flipped;
gfx_sprite_t *player2CharacterMoving2Flipped;
gfx_sprite_t *player2CharacterNotGroundedFlipped;

gfx_sprite_t *player2Weapon;
gfx_sprite_t *player2WeaponFlipped;

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
	player1CharacterNotGrounded = gfx_MallocSprite(32, 48);
	player1CharacterFlipped = gfx_MallocSprite(32, 48);
	player1CharacterMoving1Flipped = gfx_MallocSprite(32, 48);
	player1CharacterMoving2Flipped = gfx_MallocSprite(32, 48);
	player1CharacterNotGroundedFlipped = gfx_MallocSprite(32, 48);
	
	player1Weapon = gfx_MallocSprite(8, 8);
	player1WeaponFlipped = gfx_MallocSprite(8, 8);
	
	player2Character = gfx_MallocSprite(32, 48);
	player2CharacterMoving1 = gfx_MallocSprite(32, 48);
	player2CharacterMoving2 = gfx_MallocSprite(32, 48);
	player2CharacterNotGrounded = gfx_MallocSprite(32, 48);
	player2CharacterFlipped = gfx_MallocSprite(32, 48);
	player2CharacterMoving1Flipped = gfx_MallocSprite(32, 48);
	player2CharacterMoving2Flipped = gfx_MallocSprite(32, 48);
	player2CharacterNotGroundedFlipped = gfx_MallocSprite(32, 48);
	
	player2Weapon = gfx_MallocSprite(8, 8);
	player2WeaponFlipped = gfx_MallocSprite(8, 8);
	
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
	
	character0MoveSpeed = 4;
	character0FallSpeed = 4;
	character0JumpSpeed = 8;
	character0JumpHeight = 10;
			
	character0WeaponSpeed = 8;
	character0WeaponKnockback = 4;
	character0WeaponKnockbackStrength = 6;
	
	character1MoveSpeed = 8;
	character1FallSpeed = 4;
	character1JumpSpeed = 8;
	character1JumpHeight = 15;
	
	character1WeaponSpeed = 6;
	character1WeaponKnockback = 4;
	character1WeaponKnockbackStrength = 4;
	
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
			mapXBlock = 4;
			mapYBlock = 0;
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
		gfx_Tilemap(&tilemap, getXBlock(mapXBlock), getYBlock(mapYBlock));
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
	if(players == 1)
	{
		player2IsAi = 1;
	}
	else if(players == 2)
	{
		player2IsAi = 0;
	}
	switch(map)
	{
		case 0:
			player1SpawnX = 80;
			player1SpawnY = 132;
			
			player2SpawnX = 220;
			player2SpawnY = 132;
			break;
		case 1:
			player1SpawnX = 32;
			player1SpawnY = 96;
			
			player2SpawnX = 256;
			player2SpawnY = 96;
			break;
	}
	player1X = player1SpawnX;
	player1Y = player1SpawnY;
	player2X = player2SpawnX;
	player2Y = player2SpawnY;
	
	switch(player1CharacterSelection)
	{
		case 0:
			player1Character = character0;
			player1CharacterMoving1 = character0Moving1;
			player1CharacterMoving2 = character0Moving2;
			player1CharacterNotGrounded = character0NotGrounded;
			gfx_FlipSpriteY(character0, player1CharacterFlipped);
			gfx_FlipSpriteY(character0Moving1, player1CharacterMoving1Flipped);
			gfx_FlipSpriteY(character0Moving2, player1CharacterMoving2Flipped);
			gfx_FlipSpriteY(character0NotGrounded, player1CharacterNotGroundedFlipped);
			
			player1Weapon = character0Weapon;
			gfx_FlipSpriteY(character0Weapon, player1WeaponFlipped);
			
			player1MoveSpeed = character0MoveSpeed;
			player1FallSpeed = character0FallSpeed;
			player1JumpSpeed = character0JumpSpeed;
			player1JumpHeight = character0JumpHeight;
			
			player1WeaponSpeed = character0WeaponSpeed;
			player1WeaponKnockback = character0WeaponKnockback;
			player1WeaponKnockbackStrength = character0WeaponKnockbackStrength;
			break;
		case 1:
			player1Character = character1;
			player1CharacterMoving1 = character1Moving1;
			player1CharacterMoving2 = character1Moving2;
			player1CharacterNotGrounded = character1NotGrounded;
			gfx_FlipSpriteY(character1, player1CharacterFlipped);
			gfx_FlipSpriteY(character1Moving1, player1CharacterMoving1Flipped);
			gfx_FlipSpriteY(character1Moving2, player1CharacterMoving2Flipped);
			gfx_FlipSpriteY(character1NotGrounded, player1CharacterNotGroundedFlipped);
			
			player1Weapon = character1Weapon;
			gfx_FlipSpriteY(character1Weapon, player1WeaponFlipped);
			
			player1MoveSpeed = character1MoveSpeed;
			player1FallSpeed = character1FallSpeed;
			player1JumpSpeed = character1JumpSpeed;
			player1JumpHeight = character1JumpHeight;
			
			player1WeaponSpeed = character1WeaponSpeed;
			player1WeaponKnockback = character1WeaponKnockback;
			player1WeaponKnockbackStrength = character1WeaponKnockbackStrength;
			break;
	}
	switch(player2CharacterSelection)
	{
		case 0:
			player2Character = character0;
			player2CharacterMoving1 = character0Moving1;
			player2CharacterMoving2 = character0Moving2;
			player2CharacterNotGrounded = character0NotGrounded;
			gfx_FlipSpriteY(character0, player2CharacterFlipped);
			gfx_FlipSpriteY(character0Moving1, player2CharacterMoving1Flipped);
			gfx_FlipSpriteY(character0Moving2, player2CharacterMoving2Flipped);
			gfx_FlipSpriteY(character0NotGrounded, player2CharacterNotGroundedFlipped);
			
			player2Weapon = character0Weapon;
			gfx_FlipSpriteY(character0Weapon, player2WeaponFlipped);
			
			player2MoveSpeed = character0MoveSpeed;
			player2FallSpeed = character0FallSpeed;
			player2JumpSpeed = character0JumpSpeed;
			player2JumpHeight = character0JumpHeight;
			
			player2WeaponSpeed = character0WeaponSpeed;
			player2WeaponKnockback = character0WeaponKnockback;
			player2WeaponKnockbackStrength = character0WeaponKnockbackStrength;
			break;
		case 1:
			player2Character = character1;
			player2CharacterMoving1 = character1Moving1;
			player2CharacterMoving2 = character1Moving2;
			player2CharacterNotGrounded = character1NotGrounded;
			gfx_FlipSpriteY(character1, player2CharacterFlipped);
			gfx_FlipSpriteY(character1Moving1, player2CharacterMoving1Flipped);
			gfx_FlipSpriteY(character1Moving2, player2CharacterMoving2Flipped);
			gfx_FlipSpriteY(character1NotGrounded, player2CharacterNotGroundedFlipped);
			
			player2Weapon = character1Weapon;
			gfx_FlipSpriteY(character1Weapon, player2WeaponFlipped);
			
			player2MoveSpeed = character1MoveSpeed;
			player2FallSpeed = character1FallSpeed;
			player2JumpSpeed = character1JumpSpeed;
			player2JumpHeight = character1JumpHeight;
			
			player2WeaponSpeed = character1WeaponSpeed;
			player2WeaponKnockback = character1WeaponKnockback;
			player2WeaponKnockbackStrength = character1WeaponKnockbackStrength;
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
	
	while(1)
	{
		//fight - draw map
		gfx_Tilemap(&tilemap, getXBlock(mapXBlock), getYBlock(mapYBlock));
		
		//fight - get keys (player 1)
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
			if(player1Weapon1 == 0)
			{
				player1Weapon1 = 1;
				player1Weapon1Flipped = player1Flipped;
				if(player1Flipped == 0)
				{
					player1Weapon1X = player1X + 26;
				}
				else
				{
					player1Weapon1X = player1X;
				}
				player1Weapon1Y = player1Y + 17;
			}
			else if(player1Weapon2 == 0)
			{
				player1Weapon2 = 1;
				player1Weapon2Flipped = player1Flipped;
				if(player1Flipped == 0)
				{
					player1Weapon2X = player1X + 26;
				}
				else
				{
					player1Weapon2X = player1X;
				}
				player1Weapon2Y = player1Y + 17;
			}
			else if(player1Weapon3 == 0)
			{
				player1Weapon3 = 1;
				player1Weapon3Flipped = player1Flipped;
				if(player1Flipped == 0)
				{
					player1Weapon3X = player1X + 26;
				}
				else
				{
					player1Weapon3X = player1X;
				}
				player1Weapon3Y = player1Y + 17;
			}
		}
		if(key == sk_Clear)
		{
			goto pause;
		}
		
		kb_Scan();
        key = kb_Data[7];
		if(key & kb_Up)
		{
			if(testMode == 1)
			{
				player1Y -= 4;
			}
		}
		if(key & kb_Down)
		{
			if(testMode == 1)
			{
				player1Y += 4;
			}
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
		if(key & kb_Left && player1ShieldActive == 0)
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
		if(key & kb_Right && player1ShieldActive == 0)
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
		
		//fight - check for grounded and death
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
			
			playerGrounded = 0;
			playerDeath = 0;
			
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
				else if(playerY >= 240)
				{
					playerDeath = 1;
				}
			}
			else if(map == 1)
			{
				if(playerX >= -8 && playerX <= 56 && playerY == 96)
				{
					playerGrounded = 1;
				}
				else if(playerX >= 232 && playerX <= 296 && playerY == 96)
				{
					playerGrounded = 1;
				}
				else if(playerX >= 88 && playerX <= 200 && playerY == 80)
				{
					playerGrounded = 1;
				}
				else if(playerX >= 104 && playerX <= 184 && playerY == 16)
				{
					playerGrounded = 1;
				}
				else if(playerY > 160)
				{
					playerDeath = 1;
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
			
			if(playerDeath == 1)
			{
				switch(player)
				{
					case 1:
						player1Lifes--;
						if(player1Lifes <= 0)
						{
							goto gameOver;
						}
						player1X = player1SpawnX;
						player1Y = player1SpawnY;
						player1Flipped = 0;
						player1MoveAnimation = 0;
						player1MoveAnimationCount = 0;
						player1ShieldActive = 0;
						player1Grounded = 1;
						break;
					case 2:
						player2Lifes--;
						if(player2Lifes <= 0)
						{
							goto gameOver;
						}
						player2X = player2SpawnX;
						player2Y = player2SpawnY;
						player2Flipped = 1;
						player2MoveAnimation = 0;
						player2MoveAnimationCount = 0;
						player2ShieldActive = 0;
						player2Grounded = 1;
						break;
				}
			}
		}
		
		//fight - player 1 always grounded in test mode
		if(testMode == 1)
		{
			player1Grounded = 1;
		}
		
		//fight - calculate jumps (player 1)
		if(player1Jumping > 0)
		{
			player1Jumping--;
			player1Y -= player1JumpSpeed;
		}
		else if(player1Grounded == 0)
		{
			player1Y += player1FallSpeed;
		}
		
		//fight - calculate jumps (player 2)
		if(player2Jumping > 0)
		{
			player2Jumping--;
			player2Y -= player2JumpSpeed;
		}
		else if(player2Grounded == 0)
		{
			player2Y += player2FallSpeed;
		}
		
		//fight - calculate knockback (player 1)
		if(player1XKnockback < 0)
		{
			player1XKnockback++;
			player1X -= player2WeaponKnockbackStrength;
		}
		else if(player1XKnockback > 0)
		{
			player1XKnockback--;
			player1X += player2WeaponKnockbackStrength;
		}
		
		//fight - calculate knockback (player 2)
		if(player2XKnockback < 0)
		{
			player2XKnockback++;
			player2X -= player1WeaponKnockbackStrength;
		}
		else if(player2XKnockback > 0)
		{
			player2XKnockback--;
			player2X += player1WeaponKnockbackStrength;
		}
		
		//fight - calculate weapons (player 1)
		if(player1Weapon1 == 1)
		{
			if(player1Weapon1Flipped == 0)
			{
				player1Weapon1X += player1WeaponSpeed;
			}
			else
			{
				player1Weapon1X -= player1WeaponSpeed;
			}
			
			if(player1Weapon1X <= -8 || player1Weapon1X >= 320)
			{
				player1Weapon1 = 0;
			}
		}
		if(player1Weapon2 == 1)
		{
			if(player1Weapon2Flipped == 0)
			{
				player1Weapon2X += player1WeaponSpeed;
			}
			else
			{
				player1Weapon2X -= player1WeaponSpeed;
			}
			
			if(player1Weapon2X <= -8 || player1Weapon2X >= 320)
			{
				player1Weapon2 = 0;
			}
		}
		if(player1Weapon3 == 1)
		{
			if(player1Weapon3Flipped == 0)
			{
				player1Weapon3X += player1WeaponSpeed;
			}
			else
			{
				player1Weapon3X -= player1WeaponSpeed;
			}
			
			if(player1Weapon3X <= -8 || player1Weapon3X >= 320)
			{
				player1Weapon3 = 0;
			}
		}
		
		//fight - calculate weapons (player 2)
		if(player2Weapon1 == 1)
		{
			if(player2Weapon1Flipped == 0)
			{
				player2Weapon1X += player2WeaponSpeed;
			}
			else
			{
				player2Weapon1X -= player2WeaponSpeed;
			}
			
			if(player2Weapon1X <= -8 || player2Weapon1X >= 320)
			{
				player2Weapon1 = 0;
			}
		}
		if(player2Weapon2 == 1)
		{
			if(player2Weapon2Flipped == 0)
			{
				player2Weapon2X += player2WeaponSpeed;
			}
			else
			{
				player2Weapon2X -= player2WeaponSpeed;
			}
			
			if(player2Weapon2X <= -8 || player2Weapon2X >= 320)
			{
				player2Weapon2 = 0;
			}
		}
		if(player2Weapon3 == 1)
		{
			if(player2Weapon3Flipped == 0)
			{
				player2Weapon3X += player2WeaponSpeed;
			}
			else
			{
				player2Weapon3X -= player2WeaponSpeed;
			}
			
			if(player2Weapon3X <= -8 || player2Weapon3X >= 320)
			{
				player2Weapon3 = 0;
			}
		}
		
		//fight - draw character (player 1)
		if(player1Flipped == 0 && player1Grounded == 0)
		{
			gfx_TransparentSprite(player1CharacterNotGrounded, player1X, player1Y);
			player1MoveAnimation = 0;
			player1MoveAnimationCount = 0;
		}
		else if(player1Flipped == 1 && player1Grounded == 0)
		{
			gfx_TransparentSprite(player1CharacterNotGroundedFlipped, player1X, player1Y);
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
		
		//fight - draw shield (player 1)
		if(player1ShieldActive == 1)
		{
			gfx_TransparentSprite(shield, player1X - 12, player1Y - 4);
		}
		
		//fight - draw character (player 2)
		if(player2Flipped == 0 && player2Grounded == 0)
		{
			gfx_TransparentSprite(player2CharacterNotGrounded, player2X, player2Y);
			player2MoveAnimation = 0;
			player2MoveAnimationCount = 0;
		}
		else if(player2Flipped == 1 && player2Grounded == 0)
		{
			gfx_TransparentSprite(player2CharacterNotGroundedFlipped, player2X, player2Y);
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
		
		//fight - draw shield (player 2)
		if(player2ShieldActive == 1)
		{
			gfx_TransparentSprite(shield, player2X - 12, player2Y - 4);
		}
		
		//fight - draw weapons (player 1)
		if(player1Weapon1 == 1)
		{
			gfx_TransparentSprite(player1Weapon, player1Weapon1X, player1Weapon1Y);
		}
		if(player1Weapon2 == 1)
		{
			gfx_TransparentSprite(player1Weapon, player1Weapon2X, player1Weapon2Y);
		}
		if(player1Weapon3 == 1)
		{
			gfx_TransparentSprite(player1Weapon, player1Weapon3X, player1Weapon3Y);
		}
		
		//fight - draw weapons (player 2)
		if(player2Weapon1 == 1)
		{
			gfx_TransparentSprite(player2Weapon, player2Weapon1X, player2Weapon1Y);
		}
		if(player2Weapon2 == 1)
		{
			gfx_TransparentSprite(player2Weapon, player2Weapon2X, player2Weapon2Y);
		}
		if(player2Weapon3 == 1)
		{
			gfx_TransparentSprite(player2Weapon, player2Weapon3X, player2Weapon3Y);
		}
		
		//fight - check for weapon hits (player 1)
		if(player1Weapon1 == 1 && player1Weapon1Y + 8 >= player2Y && player1Weapon1Y <= player2Y + 48)
		{
			if(player1Weapon1Flipped == 0 && player1Weapon1X + 8 >= player2X && player1Weapon1X <= player2X + 32)
			{
				if(player2ShieldActive == 0)
				{
					player2XKnockback += player1WeaponKnockback;
				}
				player1Weapon1 = 0;
			}
			else if(player1Weapon1Flipped == 1 && player1Weapon1X <= player2X + 32 && player1Weapon1X + 8 >= player2X)
			{
				if(player2ShieldActive == 0)
				{
					player2XKnockback -= player1WeaponKnockback;
				}
				player1Weapon1 = 0;
			}
		}
		if(player1Weapon2 == 1 && player1Weapon2Y + 8 >= player2Y && player1Weapon2Y <= player2Y + 48)
		{
			if(player1Weapon2Flipped == 0 && player1Weapon2X + 8 >= player2X && player1Weapon2X <= player2X + 32)
			{
				if(player2ShieldActive == 0)
				{
					player2XKnockback += player1WeaponKnockback;
				}
				player1Weapon2 = 0;
			}
			else if(player1Weapon2Flipped == 1 && player1Weapon2X <= player2X + 32 && player1Weapon2X + 8 >= player2X)
			{
				if(player2ShieldActive == 0)
				{
					player2XKnockback -= player1WeaponKnockback;
				}
				player1Weapon2 = 0;
			}
		}
		if(player1Weapon3 == 1 && player1Weapon3Y + 8 >= player2Y && player1Weapon3Y <= player2Y + 48)
		{
			if(player1Weapon3Flipped == 0 && player1Weapon3X + 8 >= player2X && player1Weapon3X <= player2X + 32)
			{
				if(player2ShieldActive == 0)
				{
					player2XKnockback += player1WeaponKnockback;
				}
				player1Weapon3 = 0;
			}
			else if(player1Weapon3Flipped == 1 && player1Weapon3X <= player2X + 32 && player1Weapon3X + 8 >= player2X)
			{
				if(player2ShieldActive == 0)
				{
					player2XKnockback -= player1WeaponKnockback;
				}
				player1Weapon3 = 0;
			}
		}
		
		//fight - check for weapon hits (player 2)
		if(player2Weapon1 == 1 && player2Weapon1Y + 8 >= player1Y && player2Weapon1Y <= player1Y + 48)
		{
			if(player2Weapon1Flipped == 0 && player2Weapon1X + 8 >= player1X && player2Weapon1X <= player1X + 32)
			{
				if(player1ShieldActive == 0)
				{
					player1XKnockback += player2WeaponKnockback;
				}
				player2Weapon1 = 0;
			}
			else if(player2Weapon1Flipped == 1 && player2Weapon1X <= player1X + 32 && player2Weapon1X + 8 >= player1X)
			{
				if(player1ShieldActive == 0)
				{
					player1XKnockback -= player2WeaponKnockback;
				}
				player2Weapon1 = 0;
			}
		}
		if(player2Weapon2 == 1 && player2Weapon2Y + 8 >= player1Y && player2Weapon2Y <= player1Y + 48)
		{
			if(player2Weapon2Flipped == 0 && player2Weapon2X + 8 >= player1X && player2Weapon2X <= player1X + 32)
			{
				if(player1ShieldActive == 0)
				{
					player1XKnockback += player2WeaponKnockback;
				}
				player2Weapon2 = 0;
			}
			else if(player2Weapon2Flipped == 1 && player2Weapon2X <= player1X + 32 && player2Weapon2X + 8 >= player1X)
			{
				if(player1ShieldActive == 0)
				{
					player1XKnockback -= player2WeaponKnockback;
				}
				player2Weapon2 = 0;
			}
		}
		if(player2Weapon3 == 1 && player2Weapon3Y + 8 >= player1Y && player2Weapon3Y <= player1Y + 48)
		{
			if(player2Weapon3Flipped == 0 && player2Weapon3X + 8 >= player1X && player2Weapon3X <= player1X + 32)
			{
				if(player1ShieldActive == 0)
				{
					player1XKnockback += player2WeaponKnockback;
				}
				player2Weapon3 = 0;
			}
			else if(player2Weapon3Flipped == 1 && player2Weapon3X <= player1X + 32 && player2Weapon3X + 8 >= player1X)
			{
				if(player1ShieldActive == 0)
				{
					player1XKnockback -= player2WeaponKnockback;
				}
				player2Weapon3 = 0;
			}
		}
		
		//fight - draw UI
		gfx_TransparentSprite(statusUi, 128, 208);
		gfx_SetTextScale(2, 2);
		gfx_SetTextXY(134, 216);
		gfx_PrintInt(player1Lifes, 1);
		gfx_SetTextXY(171, 216);
		gfx_PrintInt(player2Lifes, 1);
		
		//fight - draw player 1 position
		if(testMode == 1)
		{
			gfx_SetTextXY(10, 10);
			gfx_PrintInt(player1X, 3);
			gfx_PrintString(" ");
			gfx_PrintInt(player1Y, 3);
		}
		
		//fight - swap draw
		gfx_SwapDraw();
	}
	
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
			player1Lifes = 0;
			goto gameOver;
		}
	}
	
	//---------------------------------------------------------------
	
	//gameOver
	gameOver:
	
	if(player1Lifes > 0)
	{
		gfx_Tilemap(&tilemap, getXBlock(5), getYBlock(0));
	}
	else
	{
		gfx_Tilemap(&tilemap, getXBlock(6), getYBlock(0));
	}
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