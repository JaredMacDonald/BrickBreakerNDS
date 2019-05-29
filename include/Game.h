#pragma once

class Sprite;

class Game
{
public:
	Game();
	virtual ~Game();

	void Init();
	void Run();
	void Draw();
	void Shutdown();

private:
	void ProcessInput();

	int m_Columns;
	int m_Rows;

	Sprite* m_Ball;
	Sprite* m_Paddle;
	Sprite* m_Bricks[12][5];
};