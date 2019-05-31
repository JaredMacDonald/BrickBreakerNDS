#include "ball_data.h" // gfx data
#include "Ball.h"

Ball::Ball(int x, int y)
{
	m_X = x;
	m_Y = y;
	m_Sprite = new Sprite(&oamSub, 0, ball, m_X, m_Y, BALL_WIDTH, BALL_HEIGHT, SpriteSize_8x8, SpriteColorFormat_16Color);
	m_Sprite->Init();

	m_SpeedX = 2;
	m_SpeedY = 2;
}

Ball::~Ball()
{
	if (m_Sprite != nullptr)
	{
		delete m_Sprite;
	}
}

void Ball::Draw()
{
	m_Sprite->Draw();
}

void Ball::Update()
{
	//check for collision with the screen boundries
	if (m_X <= 0 || m_X + m_Sprite->GetHeight() >= 256)
	{
		m_SpeedX = -m_SpeedX;
	}

	if (m_Y <= 0 || m_Y + m_Sprite->GetWidth() >= 192)
	{
		m_SpeedY = -m_SpeedY;
	}

	m_X += m_SpeedX;
	m_Y += m_SpeedY;

	m_Sprite->SetXPosition(m_X);
	m_Sprite->SetYPosition(m_Y);
}
