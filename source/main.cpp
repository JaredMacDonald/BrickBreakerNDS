
#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
#define BRICK 1
#define PADDLE 2

void SetupSprites(SpriteData*& sprites)
{
	sprites[BALL].gfx = oamAllocateGfx(&oamSub, SpriteSize_8x8, SpriteColorFormat_16Color);
	sprites[BALL].size = SpriteSize_8x8;
	sprites[BALL].AffineIndex = -1;
	sprites[BALL].doubleSize = false;
	sprites[BALL].x = 50;
	sprites[BALL].y = 25;

	sprites[BRICK].gfx = oamAllocateGfx(&oamSub, SpriteSize_16x8, SpriteColorFormat_16Color);
	sprites[BRICK].size = SpriteSize_16x8;
	sprites[BRICK].AffineIndex = -1;
	sprites[BRICK].doubleSize = false;
	sprites[BRICK].x = 75;
	sprites[BRICK].y = 25;

	sprites[PADDLE].gfx = oamAllocateGfx(&oamSub, SpriteSize_32x8, SpriteColorFormat_16Color);
	sprites[PADDLE].size = SpriteSize_32x8;
	sprites[PADDLE].AffineIndex = 0;
	sprites[PADDLE].doubleSize = true;
	sprites[PADDLE].x = 128;
	sprites[PADDLE].y = 160;


	dmaCopy((u8*)ball, sprites[BALL].gfx, 32);
	dmaCopy((u8*)brick, sprites[BRICK].gfx, 32 * 2);
	dmaCopy((u8*)paddle, sprites[PADDLE].gfx, 32 * 4);


	dmaCopy((u8*)SpritePalettes, SPRITE_PALETTE_SUB, 32);
}

//---------------------------------------------------------------------------------
int main(void) {
	//---------------------------------------------------------------------------------
	touchPosition touch;

	videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_0_2D);

	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankD(VRAM_D_SUB_SPRITE);
	

	// set up our bitmap background
	/*int bg = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	dmaCopy(BgImageInfo[0].data, bgGetGfxPtr(bg), 256 * 192);
	BG_PALETTE[0] = RGB15(31, 0, 0);*/

	oamInit(&oamSub, SpriteMapping_1D_32, false);

	SpriteData* sprites = new SpriteData[3];

	SetupSprites(sprites);

	int ballSpeedX = 5;
	int ballSpeedY = 5;

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

		//check for collision with the screen boundries
		if (sprites[BALL].x <= 0 || sprites[BALL].x >= 256)
			ballSpeedX = -ballSpeedX;

		if (sprites[BALL].y <= 0 || sprites[BALL].y >= 192)
			ballSpeedY = -ballSpeedY;

		for (int i = 0; i < 3; i++)
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

		swiWaitForVBlank();


		oamUpdate(&oamSub);
	}

	return 0;
}
