#pragma once

class GameObject
{
public:
	GameObject() {};
	virtual ~GameObject() {};

	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void SetXPosition(int x) { m_X = x; }
	virtual int GetXPosition() const { return m_X; }

	virtual void SetYPosition(int y) { m_Y = y; }
	virtual int GetYPosition() const { return m_Y; }

protected:
	
	int m_X;
	int m_Y;
};