#pragma once

#include <NDSEngine.h>
#include "GameObject.h"

using namespace JM;

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