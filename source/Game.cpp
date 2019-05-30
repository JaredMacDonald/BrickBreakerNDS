#include <nds.h>
#include "Game.h"
#include "Sprite.h"
#include "palettes.h"
#include "background.h"
#include "ball.h"
#include "brick.h"
#include "paddle.h"

// ---- Entry Point -------
JM::Application* JM::CreateApplication()
{
	return new Game();
}


Game::Game()
{

}

Game::~Game()
{

}

void Game::Initialize()
{
	
	m_Columns = 12;
	m_Rows = 5;

	videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_5_2D);
	
	vramSetBankA(VRAM_A_MAIN_BG);
	
	vramSetBankC(VRAM_C_SUB_BG_0x06200000);
	vramSetBankD(VRAM_D_SUB_SPRITE);
		
	
	// set up our bitmap background
	int bg = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	dmaCopy((u8*)BgImgData_background_png, bgGetGfxPtr(bg), 256 * 256);
	dmaCopy((u8*)SpritePalettes, BG_PALETTE, 32 * 3);

	dmaCopy((u8*)SpritePalettes, SPRITE_PALETTE_SUB, 32 * 3);
	
	oamInit(&oamSub, SpriteMapping_1D_32, false);

	SetupSprites();

	m_Ball->SetPosition(128, 150);
	//m_Paddle->SetPosition(128, 160);
}

void Game::Run()
{
	while (true)
	{
		touchPosition touch;

		scanKeys();

		int held = keysHeld();

		if (held & KEY_TOUCH)
		{
			touchRead(&touch);
			m_Paddle->SetXPosition(touch.px);
		}
		else if (held & KEY_LEFT)
		{
			int newX = m_Paddle->GetXPosition() - 4;
			m_Paddle->SetXPosition(newX);
			if (newX <= 0)
			{
				m_Paddle->SetXPosition(0);
			}
		}
		else if (held & KEY_RIGHT)
		{
			int newX = m_Paddle->GetXPosition() + 4;
			m_Paddle->SetXPosition(newX);
			if (m_Paddle->GetXPosition() + m_Paddle->GetHeight())
			{
				m_Paddle->SetXPosition(256 - m_Paddle->GetHeight());
			}
		}

		if (held & KEY_START)
		{
			return;
		}

		Draw();

		swiWaitForVBlank();

		oamUpdate(&oamSub);
	}
}

void Game::Draw()
{
	m_Ball->Draw();
	//m_Paddle->Draw();

	/*for (int i = 0; i < m_Columns; i++)
	{
		for (int j = 0; i < m_Rows; j++)
		{
			m_Bricks[i][j]->Draw();
		}
	}*/
}

void Game::Shutdown()
{

}

void Game::ProcessInput()
{

}

void Game::SetupSprites()
{
	m_Ball = new Sprite();
	m_Ball->Init(&oamSub, 0, ball, SpriteSize_8x8, SpriteColorFormat_16Color);

	/*m_Paddle = new Sprite();
	m_Paddle->Init(&oamSub, 1, paddle, SpriteSize_32x8, SpriteColorFormat_16Color);*/

	/*int oamIndex = 2;
	for (int i = 0; i < m_Columns; i++)
	{
		for (int j = 0; i < m_Rows; j++)
		{
			
			m_Bricks[i][j] = new Sprite();
			m_Bricks[i][j]->Init(&oamSub, oamIndex, brick, SpriteSize_16x8, SpriteColorFormat_16Color);

			oamIndex++;
		}
	}*/
}