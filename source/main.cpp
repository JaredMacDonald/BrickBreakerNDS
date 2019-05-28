
#include <nds.h>
#include <stdio.h>
#include <stdlib.h>

#include "Sprites/ball.h"
#include "Sprites/brick.h"
#include "Sprites/paddle.h"
#include "Sprites/palettes.h"
#include "Backgrounds/background.h"

struct SpriteData
{
	u16* gfx;
	SpriteSize size;
	int AffineIndex;
	bool doubleSize;
	int x;
	int y;
};

#define BALL 0
#define PADDLE 1

static const int columns = 12;
static const int rows = 5;
static int ballSpeedX = 2;
static int ballSpeedY = 2;

SpriteData bricks[16][5] = {};

void SetupSprites(SpriteData*& sprites)
{
	sprites[BALL].gfx = oamAllocateGfx(&oamSub, SpriteSize_8x8, SpriteColorFormat_16Color);
	sprites[BALL].size = SpriteSize_8x8;
	sprites[BALL].AffineIndex = -1;
	sprites[BALL].doubleSize = false;
	sprites[BALL].x = 50;
	sprites[BALL].y = 25;

	sprites[PADDLE].gfx = oamAllocateGfx(&oamSub, SpriteSize_32x8, SpriteColorFormat_16Color);
	sprites[PADDLE].size = SpriteSize_32x8;
	sprites[PADDLE].AffineIndex = 0;
	sprites[PADDLE].doubleSize = true;
	sprites[PADDLE].x = 128;
	sprites[PADDLE].y = 160;


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

            oamIndex++;
        }
    }
}

void CheckBallCollision(SpriteData& ball, SpriteData& paddle)
{
    //check for collision with the screen boundries
    if (ball.x <= 0 || ball.x >= 256)
        ballSpeedX = -ballSpeedX;

    if (ball.y <= 0 || ball.y + 8 >= 192)
        ballSpeedY = -ballSpeedY;

    // check for collision with paddle
    if (ball.y + 8 >= paddle.y && ball.x >= paddle.x && ball.x <= paddle.x + 32)
    {
        ballSpeedY = -ballSpeedY;
    }
}

//---------------------------------------------------------------------------------
int main(void) {
	//---------------------------------------------------------------------------------
	touchPosition touch;

	videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_5_2D);

	vramSetBankA(VRAM_A_MAIN_BG);

    vramSetBankC(VRAM_C_SUB_BG_0x06200000);
	vramSetBankD(VRAM_D_SUB_SPRITE);
	

	// set up our bitmap background
	int bg = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	dmaCopy((u8*)BgImgData_background_png, bgGetGfxPtr(bg), 256 * 256);
    dmaCopy((u8*)SpritePalettes, BG_PALETTE, 32 * 3);
    /*int bgSub = bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
    dmaCopy((u8*)BgImgData_background_png, bgGetGfxPtr(bgSub), 256 * 192);
    dmaCopy((u8*)SpritePalettes, BG_PALETTE_SUB, 32 * 3);*/

    

	oamInit(&oamSub, SpriteMapping_1D_32, false);
    
	SpriteData* sprites = new SpriteData[2];

	SetupSprites(sprites);
    SetupBricks();

	while (1) {

		scanKeys();

		int held = keysHeld();

		if (held & KEY_TOUCH)
		{
			touchRead(&touch);
			sprites[PADDLE].x = touch.px;
		}			

		if (held & KEY_START) break;
		

		sprites[BALL].x += ballSpeedX;
		sprites[BALL].y += ballSpeedY;

        CheckBallCollision(sprites[BALL], sprites[PADDLE]);

		for (int i = 0; i < 2; i++)
		{
			oamSet(&oamSub, //main graphics engine context
				i,           //oam index (0 to 127)  
				//touch.px, touch.py,   //x and y pixle location of the sprite
				sprites[i].x, sprites[i].y,
				0,                    //priority, lower renders last (on top)
				0,					  //this is the palette index if multiple palettes or the alpha value if bmp sprite	
				sprites[i].size,
				SpriteColorFormat_16Color,
				sprites[i].gfx,                  //pointer to the loaded graphics
				-1,                  //sprite rotation data  
				false,               //double the size when rotating?
				false,			//hide the sprite?
				false, false, //vflip, hflip
				false	//apply mosaic
			);
		}

        DrawBricks();

		swiWaitForVBlank();


		oamUpdate(&oamSub);
	}

	return 0;
}
