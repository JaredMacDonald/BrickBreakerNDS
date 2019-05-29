
#include <nds.h>
#include <stdio.h>
#include <stdlib.h>

#include "Game.h"

#include "ball.h"
#include "brick.h"
#include "paddle.h"
#include "palettes.h"
#include "background.h"

struct SpriteData
{
	u16* gfx;
	SpriteSize size;
	int AffineIndex;
	bool doubleSize;
	int x;
	int y;
    int width;
    int height;
    bool destroyed = false;
};

#define BALL 0
#define PADDLE 1

static const int columns = 12;
static const int rows = 5;
static int ballSpeedX = 2;
static int ballSpeedY = 2;

//static int paddleMovementSpeed = 4;

SpriteData bricks[16][5] = {};

void SetupSprites(SpriteData*& sprites)
{
	sprites[BALL].gfx = oamAllocateGfx(&oamSub, SpriteSize_8x8, SpriteColorFormat_16Color);
	sprites[BALL].size = SpriteSize_8x8;
	sprites[BALL].AffineIndex = -1;
	sprites[BALL].doubleSize = false;
	sprites[BALL].x = 128;
	sprites[BALL].y = 150;
    sprites[BALL].width = 8;
    sprites[BALL].height = 8;

	sprites[PADDLE].gfx = oamAllocateGfx(&oamSub, SpriteSize_32x8, SpriteColorFormat_16Color);
	sprites[PADDLE].size = SpriteSize_32x8;
	sprites[PADDLE].AffineIndex = 0;
	sprites[PADDLE].doubleSize = true;
	sprites[PADDLE].x = 128;
	sprites[PADDLE].y = 160;
    sprites[PADDLE].width = 32;
    sprites[PADDLE].height = 8;


	dmaCopy((u8*)ball, sprites[BALL].gfx, 32);
	dmaCopy((u8*)paddle, sprites[PADDLE].gfx, 32 * 4);


	dmaCopy((u8*)SpritePalettes, SPRITE_PALETTE_SUB, 32 * 3);
}

void SetupBricks()
{
    for (int i = 0; i < columns; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            bricks[i][j].gfx = oamAllocateGfx(&oamSub, SpriteSize_16x8, SpriteColorFormat_16Color);
            bricks[i][j].size = SpriteSize_16x8;
            bricks[i][j].AffineIndex = -1;
            bricks[i][j].doubleSize = false;
            bricks[i][j].x = 20 + 18 * i;
            bricks[i][j].y = 10 + 10 * j;
            bricks[i][j].width = 16;
            bricks[i][j].height = 8;
            bricks[i][j].destroyed = false;

            dmaCopy((u8*)brick, bricks[i][j].gfx, 32 * 2);
        }
    }
}

void DrawBricks()
{
    int oamIndex = 2;

    for (int i = 0; i < columns; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            SpriteData brick = bricks[i][j];

            if (brick.destroyed == false)
            {
                oamSet(&oamSub, //main graphics engine context
                    oamIndex,           //oam index (0 to 127)  
                    //touch.px, touch.py,   //x and y pixle location of the sprite
                    brick.x, brick.y,
                    0,                    //priority, lower renders last (on top)
                    0,					  //this is the palette index if multiple palettes or the alpha value if bmp sprite	
                    brick.size,
                    SpriteColorFormat_16Color,
                    brick.gfx,                  //pointer to the loaded graphics
                    -1,                  //sprite rotation data  
                    false,               //double the size when rotating?
                    false,			//hide the sprite?
                    false, false, //vflip, hflip
                    false	//apply mosaic
                );

                if (oamIndex % 2 == 0)
                {
                    oamSetPalette(&oamSub, oamIndex, 2);
                }
            }
            else
            {
                oamClearSprite(&oamSub, oamIndex);
            }            

            oamIndex++;
        }
    }
}

void CheckBallCollisionWithScreen(SpriteData& ball)
{
    //check for collision with the screen boundries
    if (ball.x <= 0 || ball.x >= 256)
        ballSpeedX = -ballSpeedX;

    if (ball.y <= 0 || ball.y + 8 >= 192)
        ballSpeedY = -ballSpeedY;
}

void CheckBallCollisionWithPaddle(SpriteData& ball, SpriteData& paddle)
{
    // check for collision with paddle
    if (ball.y + ball.height >= paddle.y && ball.x >= paddle.x && ball.x + ball.width <= paddle.x + paddle.width)
    {
        ballSpeedY = -ballSpeedY;
    }
}

void CheckBallCollisionWithBrick(SpriteData& ball, SpriteData& brick)
{
    if (brick.destroyed)
    {
        return;
    }

    // check for collision with paddle
    if (ball.x >= brick.x && ball.x <= brick.x + brick.width && 
        ball.y + ball.height >= brick.y && ball.y + ball.height <= brick.y + brick.height)
    {
        ballSpeedY = -ballSpeedY;

        brick.destroyed = true;
    }
}

static Game* g_Game;
//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	g_Game = new Game();
	if (!g_Game)
	{
		// Something went wrong and unable to create game.
		return 0;
	}

	g_Game->Init();

	videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_5_2D);

	vramSetBankA(VRAM_A_MAIN_BG);

    vramSetBankC(VRAM_C_SUB_BG_0x06200000);
	vramSetBankD(VRAM_D_SUB_SPRITE);
	

	// set up our bitmap background
	int bg = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	dmaCopy((u8*)BgImgData_background_png, bgGetGfxPtr(bg), 256 * 256);
    dmaCopy((u8*)SpritePalettes, BG_PALETTE, 32 * 3);

	oamInit(&oamSub, SpriteMapping_1D_32, false);
    
	SpriteData* sprites = new SpriteData[2];

	SetupSprites(sprites);
    SetupBricks();

	while (1) {

		g_Game->Run();

		//scanKeys();

		//int held = keysHeld();

  //      if (held & KEY_TOUCH)
  //      {
  //          touchRead(&touch);
  //          sprites[PADDLE].x = touch.px;
  //      }
  //      else if (held & KEY_LEFT)
  //      {
  //          sprites[PADDLE].x -= paddleMovementSpeed;
  //          if (sprites[PADDLE].x <= 0)
  //          {
  //              sprites[PADDLE].x = 0;
  //          }
  //      }
  //      else if (held & KEY_RIGHT)
  //      {
  //          sprites[PADDLE].x += paddleMovementSpeed;
  //          if (sprites[PADDLE].x + sprites[PADDLE].width >= 256)
  //          {
  //              sprites[PADDLE].x = 256 - sprites[PADDLE].width;
  //          }
  //      }

  //      if (held & KEY_START)
  //      {
  //          break;
  //      }
		//

		//sprites[BALL].x += ballSpeedX;
		//sprites[BALL].y += ballSpeedY;

  //      CheckBallCollisionWithScreen(sprites[BALL]);
  //      CheckBallCollisionWithPaddle(sprites[BALL], sprites[PADDLE]);

  //      for (int x = 0; x < columns; x++)
  //      {
  //          for (int y = 0; y < rows; y++)
  //          {
  //              CheckBallCollisionWithBrick(sprites[BALL], bricks[x][y]);
  //          }
  //      }

		//for (int i = 0; i < 2; i++)
		//{
		//	oamSet(&oamSub, //main graphics engine context
		//		i,           //oam index (0 to 127)  
		//		//touch.px, touch.py,   //x and y pixle location of the sprite
		//		sprites[i].x, sprites[i].y,
		//		0,                    //priority, lower renders last (on top)
		//		0,					  //this is the palette index if multiple palettes or the alpha value if bmp sprite	
		//		sprites[i].size,
		//		SpriteColorFormat_16Color,
		//		sprites[i].gfx,                  //pointer to the loaded graphics
		//		-1,                  //sprite rotation data  
		//		false,               //double the size when rotating?
		//		false,			//hide the sprite?
		//		false, false, //vflip, hflip
		//		false	//apply mosaic
		//	);
		//}

  //      DrawBricks();

		//swiWaitForVBlank();

  //      oamUpdate(&oamSub);

	}

	return 0;
}
