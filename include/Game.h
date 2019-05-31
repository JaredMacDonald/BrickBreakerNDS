#pragma once
#include <vector>
#include <NDSEngine.h>

class GameObject;

class Game : public JM::Application
{
public:
	Game();
	virtual ~Game();

	virtual void Initialize();
	virtual void Run();

	void Update();
	void Draw();
	virtual void Shutdown();

private:
	void ProcessInput();

	void SetupSprites();

	int m_Columns;
	int m_Rows;

	//Ball* m_Ball;
	JM::Sprite* m_Paddle;
	JM::Sprite* m_Bricks[12][5];

	std::vector<GameObject*> m_GameObjects;
};