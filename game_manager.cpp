#include "game_manager.h"

GameManager::GameManager() {
	clock = new sf::Clock();
	paddle = new Paddle(WINDOW_WIDTH);
	ball = new Ball(WINDOW_WIDTH, paddle->getPosition().y, 0.f);
	ball->SetDepartureSpeedToDefault();
	blocks = new BlockSet();
	bonus = new Bonus();
	lives = new LiveBar();
	score = new Score();
	bottom = new Bottom();
	secondBall = new Ball(WINDOW_WIDTH, paddle->getPosition().y, ball->getSize().x + 1);
	window.setFramerateLimit(FRAMERATE);
	SetDefaultBackground();
}

GameManager::~GameManager() {
	delete clock;
	delete paddle;
	delete ball;
	delete blocks;
	delete bonus;
	delete lives;
	delete score;
	delete bottom;
	delete secondBall;
}

void GameManager::SetDefaultBackground() {
	backgroundTexture.loadFromFile("resources\\background.png");
	background.setTexture(backgroundTexture);
}

void GameManager::SetGameOverBackground() {
	backgroundTexture.loadFromFile("resources\\gameover.png");
	background.setTexture(backgroundTexture);
}

void GameManager::SetWinBackground() {
	backgroundTexture.loadFromFile("resources\\win.png");
	background.setTexture(backgroundTexture);
}

void GameManager::RestartGame() {
	SetDefaultBackground();
	state = GAME_STATE::PLAY;
	blocks->RebuildSet();
	ball->setPosition((static_cast<float>(WINDOW_WIDTH) - ball->getSize().x) / 2, paddle->getPosition().y - ball->getSize().y);
	ball->isOnPaddle = true;
	ball->SetDepartureSpeedToDefault();
	ball->SetDepartureSpeed();
	paddle->setPosition((static_cast<float>(WINDOW_WIDTH) - paddle->getSize().x) / 2, paddle->getPosition().y);
	paddle->SetDefaultSize();

	paddle->isSticky = false;
	bottom->isActivated = false;
	blocks->isMovingBlockActivated = false;
	isSecondBall = false;
	bonus->isOnGameField = false;

	score->Reset();
	lives->Reset();
}

void GameManager::DrawGame() {
	window.clear();
	window.draw(background);
	window.draw(*ball);
	if (isSecondBall)
		window.draw(*secondBall);
	window.draw(*paddle);
	window.draw(*score);
	lives->Draw(window);
	if (bonus->isOnGameField)
		window.draw(*bonus);
	blocks->Draw(window);
	if (bottom->isActivated)
		window.draw(*bottom);
	window.display();
}

void GameManager::DrawEndGame() {
	window.clear();
	window.draw(background);
	window.draw(*score);
	lives->Draw(window);
	window.display();
}

void GameManager::ProcessKeyPress() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		paddle->direction = DIRECTION::LEFT;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		paddle->direction = DIRECTION::RIGHT;
	else
		paddle->direction = DIRECTION::NONE;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
		if (ball->isOnPaddle) {
			ball->SetDepartureSpeed();
			ball->isOnPaddle = false;
		}
		if (isSecondBall && secondBall->isOnPaddle) {
			secondBall->SetDepartureSpeed();
			secondBall->isOnPaddle = false;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
		RestartGame();
}

void GameManager::CheckForBallFall() {
	if (ball->IsFall(WINDOW_HEIGHT)) {
		if (!isSecondBall) {
			ball->isOnPaddle = true;
			score->Reduce();
			if (!lives->ProcessFall()) {
				SetGameOverBackground();
				state = GAME_STATE::GAME_OVER;
			}
		}
		else {
			std::swap(ball, secondBall);
			ball->SetPaddleCenterShift(0.f);
			secondBall->SetPaddleCenterShift(ball->getSize().x + 1);
		}
	}
	if (isSecondBall && secondBall->IsFall(WINDOW_HEIGHT))
		isSecondBall = false;
}

void GameManager::CheckForWin() {
	if (blocks->IsBlocksAreOver()) {
		SetWinBackground();
		state = GAME_STATE::WIN;
	}
}

void GameManager::ProcessBallBorderCollision(Ball* _ball) {
	if (_ball->GetLeftBound() <= BORDER_WIDTH) {
		_ball->setPosition({ static_cast<float>(BORDER_WIDTH), _ball->getPosition().y });
		_ball->ReverseXSpeed();
	}
	else if (_ball->GetRightBound() >= WINDOW_WIDTH - BORDER_WIDTH) {
		_ball->setPosition({ static_cast<float>(WINDOW_WIDTH - BORDER_WIDTH) - _ball->getSize().x, _ball->getPosition().y });
		_ball->ReverseXSpeed();
	}
	else if (_ball->GetTopBound() < BORDER_WIDTH) {
		_ball->setPosition({ _ball->getPosition().x, static_cast<float>(BORDER_WIDTH) });
		_ball->ReverseYSpeed();
	}

	else if (bottom->isActivated && _ball->getGlobalBounds().intersects(bottom->getGlobalBounds())) {
		_ball->setPosition({ _ball->getPosition().x, bottom->GetTopBound() - _ball->getSize().y });
		_ball->ReverseYSpeed();
		bottom->isActivated = false;
	}
}

void GameManager::ProcessBallPaddleCollision(Ball* _ball) {
	if (!_ball->getGlobalBounds().intersects(paddle->getGlobalBounds()) || _ball->isOnPaddle)
		return;

	if (paddle->isSticky) {
		_ball->isOnPaddle = true;
		return;
	}

	if (_ball->GetTopBound() >= paddle->GetBottomBound())
		_ball->SetSpeed(_ball->GetSpeed().x, abs(_ball->GetSpeed().y));
	else
		_ball->SetSpeed(_ball->GetSpeed().x, -1 * abs(_ball->GetSpeed().y));

	float ballX = _ball->getPosition().x;
	float ballY = _ball->getPosition().y;
	float ballSize = _ball->getSize().x;
	float paddleX = paddle->getPosition().x;
	float paddleY = paddle->getPosition().y;
	float paddleWidth = paddle->getSize().x;
	float paddleHeight = paddle->getSize().y;

	bool topIntersectRight = sf::FloatRect(ballX, ballY, ballSize, 1).intersects(sf::FloatRect(paddleX + paddleWidth, paddleY, 1, paddleHeight));
	bool topIntersectLeft = sf::FloatRect(ballX, ballY, ballSize, 1).intersects(sf::FloatRect(paddleX, paddleY, 1, paddleHeight));
	bool bottomIntersectRight = sf::FloatRect(ballX, ballY + ballSize, ballSize, 1).intersects(sf::FloatRect(paddleX + paddleWidth, paddleY, 1, paddleHeight));
	bool bottomIntersectLeft = sf::FloatRect(ballX, ballY + ballSize, ballSize, 1).intersects(sf::FloatRect(paddleX, paddleY, 1, paddleHeight));

	//if the ball collided with the vertical part of the paddle
	if (topIntersectRight || topIntersectLeft || bottomIntersectRight || bottomIntersectLeft)
		_ball->ReverseXSpeed();
	//if the ball collided only with the horizontal part of the paddle
	else {
		//changing the direction of the ball depending on the part of the paddle that was encountered
		float sign;
		if (paddle->GetSpeed().x > 0 || (paddle->GetSpeed().x == 0 && _ball->GetSpeed().x > 0))
			sign = 1.f;
		else if (paddle->GetSpeed().x < 0 || (paddle->GetSpeed().x == 0 && _ball->GetSpeed().x < 0))
			sign = -1.f;
		else
			sign = 1.f;

		float coefficient = sign * abs((paddle->getPosition().x + paddle->getSize().x / 2) - (_ball->getPosition().x + _ball->getSize().x / 2)) / (paddle->getSize().x / 2);
		_ball->ChangeSpeedAfterPaddleColision(coefficient);
	}
}

void GameManager::ProcessBallBlockSetCollision(Ball* _ball, float time) {
	if (blocks->isMovingBlockActivated && !blocks->movingBlock->isDestroyed && _ball->getGlobalBounds().intersects(blocks->movingBlock->getGlobalBounds())) {
		ProcessOneBlockCollision(_ball, blocks->movingBlock, time);
		blocks->movingBlock->ManageCollision(score);
		if (blocks->movingBlock->isDestroyed) {
			blocks->ReduceBlockNumber();
			blocks->isMovingBlockActivated = false;
		}
	}
	if (!blocks->GetRectangle().intersects(_ball->getGlobalBounds()))
		return;

	for (int i = 0; i < blocks->GetBlocksNumber().x; ++i)
		for (int j = 0; j < blocks->GetBlocksNumber().y; ++j) {
			if (blocks->set[i][j]->isDestroyed || !_ball->getGlobalBounds().intersects(blocks->set[i][j]->getGlobalBounds()))
				continue;
			ProcessOneBlockCollision(_ball, blocks->set[i][j], time);
			if (blocks->set[i][j]->ManageCollision(score)) //if block speeds ball up
				_ball->SpeedUp();

			if (blocks->set[i][j]->isDestroyed)
				blocks->ReduceBlockNumber();

			if (blocks->set[i][j]->hasBonus == true && bonus->isOnGameField == false)
				bonus->GetBonus(blocks->set[i][j]->getPosition(), blocks->set[i][j]->getSize());
		}
}

void GameManager::ProcessOneBlockCollision(Ball* _ball, const Block* block, float time) {
	float ballX = _ball->getPosition().x;
	float ballY = _ball->getPosition().y;
	float ballSize = _ball->getSize().x;
	float blockX = block->getPosition().x;
	float blockY = block->getPosition().y;
	float blockWidth = block->getSize().x;
	float blockHeight = block->getSize().y;

	//checking which borders are crossed to understand which side the ball came from
	bool topIntersectRight = sf::FloatRect(ballX, ballY, ballSize, 1).intersects(sf::FloatRect(blockX + blockWidth, blockY, 1, blockHeight));
	bool topIntersectLeft = sf::FloatRect(ballX, ballY, ballSize, 1).intersects(sf::FloatRect(blockX, blockY, 1, blockHeight));
	bool bottomIntersectRight = sf::FloatRect(ballX, ballY + ballSize, ballSize, 1).intersects(sf::FloatRect(blockX + blockWidth, blockY, 1, blockHeight));
	bool bottomIntersectLeft = sf::FloatRect(ballX, ballY + ballSize, ballSize, 1).intersects(sf::FloatRect(blockX, blockY, 1, blockHeight));

	bool leftIntersectTop = sf::FloatRect(ballX, ballY, 1, ballSize).intersects(sf::FloatRect(blockX, blockY, blockWidth, 1));
	bool leftIntersectBottom = sf::FloatRect(ballX, ballY, 1, ballSize).intersects(sf::FloatRect(blockX, blockY + blockHeight, blockWidth, 1));
	bool rightIntersectTop = sf::FloatRect(ballX + ballSize, ballY, 1, ballSize).intersects(sf::FloatRect(blockX, blockY, blockWidth, 1));
	bool rightIntersectBottom = sf::FloatRect(ballX + ballSize, ballY, 1, ballSize).intersects(sf::FloatRect(blockX, blockY + blockHeight, blockWidth, 1));

	_ball->setPosition(_ball->getPosition().x - _ball->GetSpeed().x * time, _ball->getPosition().y - _ball->GetSpeed().y * time);

	if (topIntersectRight || topIntersectLeft || bottomIntersectRight || bottomIntersectLeft)
		_ball->ReverseXSpeed();
	if (leftIntersectTop || leftIntersectBottom || rightIntersectTop || rightIntersectBottom)
		_ball->ReverseYSpeed();
}

void GameManager::ManageBallCollisions(Ball* _ball, float time) {
	ProcessBallBlockSetCollision(_ball, time);
	ProcessBallBorderCollision(_ball);
	ProcessBallPaddleCollision(_ball);
}

void GameManager::ProcessTwoBallsColision() {
	if (!ball->getGlobalBounds().intersects(secondBall->getGlobalBounds()) || ball->isOnPaddle || secondBall->isOnPaddle)
		return;
	ball->ReverseXSpeed();
	secondBall->ReverseXSpeed();
}

void GameManager::ActivateSecondBall() {
	if (isSecondBall)
		return;
	isSecondBall = true;
	secondBall->FollowPaddle(paddle);
	if (ball->isOnPaddle)
		secondBall->isOnPaddle = true;
	else {
		secondBall->isOnPaddle = false;
		secondBall->SetDepartureSpeedToDefault();
		secondBall->SetDepartureSpeed();
	}
}

void GameManager::ManageBonus(float time) {
	bottom->ManageActiveTime(time);
	paddle->ManageBonusActiveTime(time, WINDOW_WIDTH, BORDER_WIDTH);

	if (!bonus->isOnGameField)
		return;

	//returns true if bonus must be activated
	if (!bonus->UpdatePosition(paddle, WINDOW_HEIGHT, time))
		return;

	//activating new bonus
	BONUS_TYPE bonusType = bonus->GetBonusType();
	switch (bonusType) {

	case BONUS_TYPE::ENLARGE_PADDLE:
		paddle->EnlargePaddle(WINDOW_WIDTH, BORDER_WIDTH);
		break;

	case BONUS_TYPE::REDUCE_PADDLE:
		paddle->ReducePaddle();
		break;

	case BONUS_TYPE::STICKY_PADDLE:
		paddle->MakeSticky();
		break;

	case BONUS_TYPE::SECOND_BALL:
		ActivateSecondBall();
		break;

	case BONUS_TYPE::SPEED_BALL_UP:
		ball->SpeedUp();
		if (isSecondBall)
			secondBall->SpeedUp();
		break;

	case BONUS_TYPE::SLOW_BALL_DOWN:
		ball->SlowDown();
		if (isSecondBall)
			secondBall->SlowDown();
		break;

	case BONUS_TYPE::CHANGE_TRAJECTORY:
		ball->GetSpeed().x > ball->GetSpeed().y ? ball->ReverseXSpeed() : ball->ReverseYSpeed();
		if (isSecondBall)
			secondBall->GetSpeed().x > secondBall->GetSpeed().y ? secondBall->ReverseXSpeed() : secondBall->ReverseYSpeed();
		break;

	case BONUS_TYPE::MOVING_BLOCK:
		blocks->ActivateMovingBlock();
		break;

	case BONUS_TYPE::BOTTOM_BOUND:
		bottom->Activate();
		break;
	}
}

void GameManager::RunGame() {
	while (window.isOpen()) {
		float time = static_cast<float>(clock->getElapsedTime().asMicroseconds());
		clock->restart();
		time = time / FREQUENCY;

		sf::Event event;
		window.pollEvent(event);
		if (event.type == sf::Event::Closed)
			window.close();

		if (state != GAME_STATE::PLAY) {
			DrawEndGame();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
				RestartGame();
			continue;
		}

		ProcessKeyPress();

		paddle->UpdatePosition(WINDOW_WIDTH, BORDER_WIDTH, time);
		ball->UpdatePosition(paddle, time);
		blocks->UpdateMovingBlockPosition(time);
		ManageBallCollisions(ball, time);

		if (isSecondBall) {
			secondBall->UpdatePosition(paddle, time);
			ManageBallCollisions(secondBall, time);
			ProcessTwoBallsColision();
		}

		ManageBonus(time);
		CheckForBallFall();
		CheckForWin();
		DrawGame();
	}
}
