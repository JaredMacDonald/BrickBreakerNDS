#pragma once

class GameObject
{
public:
	GameObject() {};
	virtual ~GameObject() {};

	virtual void Update() = 0;
	virtual void Draw() = 0;

protected:
	
	int m_X;
	int m_Y;
};