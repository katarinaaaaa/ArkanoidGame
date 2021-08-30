#pragma once
#include "object.h"

enum class DIRECTION {
	NONE,
	LEFT,
	RIGHT,
};

class Paddle : public Object {
	const float PADDLE_WIDTH = 123.f;
	const float PADDLE_HEIGHT = 24.f;
	const float PADDLE_MOVE_SPEED = 0.5f;
	const float PADDLE_Y = 700.f;
	const float RESIZE_COEF = 1.3f;
	const float BONUS_ACTIVE_TIME = 8000.f;

	float stickyActiveTimeCounter = 0;
	float resizeActiveTimeCounter = 0;

public:
	bool isSticky = false;
	DIRECTION direction;

	explicit Paddle(int windowWidth);
	void UpdatePosition(float windowWidth, float borderWidth, float time);

	void MakeSticky();
	void EnlargePaddle(float windowWidth, float borderWidth);
	void ReducePaddle();
	void SetDefaultSize();
	void ManageBonusActiveTime(float time, float windowWidth, float borderWidth);
};
