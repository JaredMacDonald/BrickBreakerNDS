#pragma once

#include <Render/2D/Sprite.h>

using namespace JM;

#include "GameObject.h"

class Paddle : public GameObject
{
public:
	Paddle(int x, int y);
	virtual ~Paddle();

	void Update() override;
	void Draw() override;

private:
	int m_MovementSpeed;

	Sprite* m_Sprite;
};