#include <NDSEngine.h>

#include "paddle_data.h" // gfx
#include "Paddle.h"

using namespace JM;

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
	touchPosition touch;
	InputManager* inputManager = InputManager::GetInstance();
	if (inputManager == nullptr)
		return;

	if (inputManager->IsButtonHeld(BUTTON_TOUCH))
	{
		touchRead(&touch);
		SetXPosition(touch.px);
	}
	else if (inputManager->IsButtonHeld(BUTTON_DPAD_LEFT))
	{
		int newX = GetXPosition() - 4;
		SetXPosition(newX);
		if (newX <= 0)
		{
			SetXPosition(newX);
		}
	}
	else if (inputManager->IsButtonHeld(BUTTON_DPAD_RIGHT))
	{
		int newX = GetXPosition() + 4;
		SetXPosition(newX);
		if (GetXPosition() >= 256)
		{
			SetXPosition(256 - 8);
		}
	}

	m_Sprite->SetPosition(m_X, m_Y);
}