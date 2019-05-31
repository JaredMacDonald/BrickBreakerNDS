#include "paddle_data.h" // gfx
#include "Paddle.h"

Paddle::Paddle(int x, int y)
{
	m_X = x;
	m_Y = y;
	
	m_MovementSpeed = 4;

	m_Sprite = new Sprite(&oamSub, 1, paddle, m_X, m_Y, 32, 8, SpriteSize_32x8, SpriteColorFormat_16Color);
	m_Sprite->Init();
}

Paddle::~Paddle()
{

}

void Paddle::Draw()
{
	m_Sprite->Draw();
}

void Paddle::Update()
{

}