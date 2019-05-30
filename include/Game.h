#pragma once
#include <NDSEngine.h>

class Sprite;

class Game : public JM::Application
{
public:
	Game();
	virtual ~Game();

	virtual void Initialize();
	virtual void Run();

	void Draw();
	virtual void Shutdown();

private:
	void ProcessInput();

	void SetupSprites();

	int m_Columns;
	int m_Rows;

	Sprite* m_Ball;
	Sprite* m_Paddle;
	Sprite* m_Bricks[12][5];
};