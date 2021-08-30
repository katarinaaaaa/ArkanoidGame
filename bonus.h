#pragma once
#include "object.h"
#include "paddle.h"

enum class BONUS_TYPE {
	ENLARGE_PADDLE,
	REDUCE_PADDLE,
	STICKY_PADDLE,
	SECOND_BALL,
	SPEED_BALL_UP,
	SLOW_BALL_DOWN,
	CHANGE_TRAJECTORY,
	MOVING_BLOCK,
	BOTTOM_BOUND,
	TYPES_NUM,
	NONE,
};

class Bonus : public Object {
	const float BONUS_WIDTH = 42.f;
	const float BONUS_HEIGHT = 25.f;
	const float BONUS_TEXTURE_GAP = 2.f;
	const float BONUS_MOVE_SPEED = 0.2f;

	BONUS_TYPE type = BONUS_TYPE::NONE;

	void SetNewRandomType();

public:
	bool isOnGameField = false;

	Bonus();

	void GetBonus(const sf::Vector2f& blockPosition, const sf::Vector2f& blockSize);
	bool UpdatePosition(const Paddle* paddle, int windowHeight, float time); //returns true if bonus must be activated
	BONUS_TYPE GetBonusType();
};
