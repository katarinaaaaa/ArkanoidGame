#include "paddle.h"

Paddle::Paddle(int windowWidth) {
	setPosition((static_cast<float>(windowWidth) - PADDLE_WIDTH) / 2, PADDLE_Y);
	setSize({ PADDLE_WIDTH, PADDLE_HEIGHT });
	texture.loadFromFile("resources\\paddle.png");
	setTexture(&texture);
	speed = { 0.f, 0.f };
	direction = DIRECTION::NONE;
}

void Paddle::UpdatePosition(float windowWidth, float borderWidth, float time) {
	if (direction == DIRECTION::LEFT && GetLeftBound() >= borderWidth)
		speed.x = -PADDLE_MOVE_SPEED;
	else if (direction == DIRECTION::RIGHT && GetRightBound() <= windowWidth - borderWidth)
		speed.x = PADDLE_MOVE_SPEED;
	else
		speed.x = 0.f;
	move(speed * time);
	direction = DIRECTION::NONE;
}

void Paddle::MakeSticky() {
	isSticky = true;
	stickyActiveTimeCounter = 0;
}

void Paddle::EnlargePaddle(float windowWidth, float borderWidth) {
	if (getSize().x > PADDLE_WIDTH) {
		resizeActiveTimeCounter = 0;
		return;
	}
	setSize({ PADDLE_WIDTH * RESIZE_COEF, PADDLE_HEIGHT });
	float shift = GetRightBound() - (windowWidth - borderWidth);
	if (shift > 0)
		move(-shift, 0.f);
	resizeActiveTimeCounter = 0;
}

void Paddle::ReducePaddle() {
	if (getSize().x < PADDLE_WIDTH) {
		resizeActiveTimeCounter = 0;
		return;
	}
	setSize({ PADDLE_WIDTH / RESIZE_COEF, PADDLE_HEIGHT });
	resizeActiveTimeCounter = 0;
}

void Paddle::SetDefaultSize() {
	setSize({ PADDLE_WIDTH, PADDLE_HEIGHT });
}

void Paddle::ManageBonusActiveTime(float time, float windowWidth, float borderWidth) {
	if (isSticky) {
		stickyActiveTimeCounter += time;
		if (stickyActiveTimeCounter >= BONUS_ACTIVE_TIME)
			isSticky = false;
	}

	if (getSize().x > PADDLE_WIDTH) { //paddle is enlarged
		resizeActiveTimeCounter += time;
		if (resizeActiveTimeCounter >= BONUS_ACTIVE_TIME)
			SetDefaultSize();
	}
	else if (getSize().x < PADDLE_WIDTH) { //paddle is reduced
		resizeActiveTimeCounter += time;
		if (resizeActiveTimeCounter >= BONUS_ACTIVE_TIME) {
			SetDefaultSize();
			float shift = GetRightBound() - (windowWidth - borderWidth);
			if (shift > 0)
				move(-shift, 0.f);
		}
	}
}
