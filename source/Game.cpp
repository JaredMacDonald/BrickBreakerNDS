#include <nds.h>
#include "../include/Game.h"
#include "../include/Sprites/Sprite.h"

Game::Game()
{

}

Game::~Game()
{

}

void Game::Init()
{
	m_Columns = 12;
	m_Rows = 5;
}

void Game::Run()
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
		//m_Paddle->SetXPosition(newX);
		if (newX <= 0)
		{
			//m_Paddle->SetXPosition(0);
		}
	}
	else if (held & KEY_RIGHT)
	{
		//int newX = m_Paddle->GetXPosition() + 4;
		//m_Paddle->SetXPosition(newX);
		if (m_Paddle->GetXPosition() + m_Paddle->GetHeight())
		{
			//m_Paddle->SetXPosition(256 - m_Paddle->GetHeight());
		}
	}

	if (held & KEY_START)
	{
		return;
	}


	//m_Ball->MovePosition(2, 2);
	//sprites[BALL].x += ballSpeedX;
	//sprites[BALL].y += ballSpeedY;

	//CheckBallCollisionWithScreen(m_Ball);
	//CheckBallCollisionWithPaddle(sprites[BALL], sprites[PADDLE]);

	/*for (int x = 0; x < m_Columns; x++)
	{
		for (int y = 0; y < m_Rows; y++)
		{
			CheckBallCollisionWithBrick(sprites[BALL], bricks[x][y]);
		}
	}*/

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

	//DrawBricks();

	swiWaitForVBlank();

	oamUpdate(&oamSub);

}

void Game::Draw()
{
	//m_Ball->Draw();
	//m_Paddle->Draw();

	for (int i = 0; i < m_Columns; i++)
	{
		for (int j = 0; i < m_Rows; j++)
		{
			//m_Bricks[i][j]->Draw();
		}
	}
}

void Game::Shutdown()
{

}

void Game::ProcessInput()
{

}