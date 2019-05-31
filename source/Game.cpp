#include <nds.h>
#include <NDSEngine.h>

#include <Ball.h>
#include <Game.h>
#include <GameObject.h>
#include <Paddle.h>

#include <palettes.h>
#include <background.h>
#include <brick_data.h>
#include <paddle_data.h>

using namespace JM;

// ---- Entry Point -------
JM::Application* JM::CreateApplication()
{
	return new Game();
}
// -----------------------

Game::Game()
{

}

Game::~Game()
{
	Shutdown();
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

		Update();

		Draw();
		
		swiWaitForVBlank();
		oamUpdate(&oamSub);
	}
}

void Game::Draw()
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Draw();
	}
}

void Game::Update()
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Update();
	}
}

void Game::Shutdown()
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		delete m_GameObjects[i];
	}

	//delete m_Ball;
}

void Game::ProcessInput()
{

}

void Game::SetupSprites()
{
	/*m_Ball = ;
	m_Paddle = ;*/

	m_GameObjects.push_back(new Ball(128, 150));
	m_GameObjects.push_back(new Paddle(128, 160));

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