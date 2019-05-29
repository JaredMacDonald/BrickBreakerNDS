#include <nds.h>
#include "Game.h"
#include "Sprite.h"

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

	swiWaitForVBlank();

	oamUpdate(&oamSub);

}

void Game::Draw()
{
	m_Ball->Draw();
	m_Paddle->Draw();

	for (int i = 0; i < m_Columns; i++)
	{
		for (int j = 0; i < m_Rows; j++)
		{
			m_Bricks[i][j]->Draw();
		}
	}
}

void Game::Shutdown()
{

}

void Game::ProcessInput()
{

}