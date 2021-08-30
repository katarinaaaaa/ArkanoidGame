#pragma once
#include "paddle.h"
#include "ball.h"
#include "block_set.h"
#include "bonus.h"
#include "live_bar.h"
#include "score.h"
#include "bottom.h"

enum class GAME_STATE {
	PLAY,
	WIN,
	GAME_OVER,
};

class GameManager {
	static const int WINDOW_WIDTH = 1000;
	static const int WINDOW_HEIGHT = 750;
	static const int BORDER_WIDTH = 18;
	static const int FRAMERATE = 200;
	static const int FREQUENCY = 800;

	sf::Texture backgroundTexture;
	sf::Sprite background;

	GAME_STATE state = GAME_STATE::PLAY;

	sf::Clock* clock;

	Paddle* paddle;
	Ball* ball;
	BlockSet* blocks;
	Bonus* bonus;

	LiveBar* lives;
	Score* score;

	Bottom* bottom;
	Ball* secondBall;
	bool isSecondBall = false;

	void SetDefaultBackground();
	void SetGameOverBackground();
	void SetWinBackground();
	void RestartGame();

	void DrawGame();
	void DrawEndGame();

	void ProcessKeyPress();
	void CheckForBallFall();
	void CheckForWin();

	void ProcessBallBorderCollision(Ball* _ball);
	void ProcessBallPaddleCollision(Ball* _ball);
	void ProcessBallBlockSetCollision(Ball* _ball, float time);
	void ProcessOneBlockCollision(Ball* _ball, const Block* block, float time);
	void ManageBallCollisions(Ball* _ball, float time);
	void ProcessTwoBallsColision();

	void ActivateSecondBall();
	void ManageBonus(float time);

public:
	sf::RenderWindow window { sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "arkanoid", sf::Style::Close };

	GameManager();
	~GameManager();
	void RunGame();
};
