#pragma once

#include <Render/2D/Sprite.h>

#include "GameObject.h"

using namespace JM;

#define BALL_WIDTH 8
#define BALL_HEIGHT 8

class Ball : public GameObject
{
public:
	Ball(int x, int y);
	~Ball();

	void Update() override;
	void Draw() override;

private:
	int m_SpeedX;
	int m_SpeedY;
	
	Sprite* m_Sprite;
};