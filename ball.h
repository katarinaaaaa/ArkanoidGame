#pragma once
#include "object.h"
#include "paddle.h"

class Ball : public Object {
	const float BALL_SIZE = 24.f;
	const float SPEED_UP_COEF = 1.1f;
	const float SLOW_DOWN_COEF = 0.9f;
	const sf::Vector2f DEFAULT_SPEED { 0.3f, -0.3f };
	sf::Vector2f DEPARTURE_SPEED = DEFAULT_SPEED; //started speed at which the ball flies out of the paddle
	float paddleCenterShift; //for second ball

public:
	bool isOnPaddle;

	Ball(float windowWidth, float paddleY, float shift);

	void FollowPaddle(const Paddle* paddle);
	void UpdatePosition(const Paddle* paddle, float time);
	bool IsFall(int windowHeight) const;

	void ChangeSpeedAfterPaddleColision(float coefficient); //increases the x-component of the speed when colliding with the paddle
	void SetDepartureSpeedToDefault();
	void SetDepartureSpeed();
	void SpeedUp();
	void SlowDown();
	void SetPaddleCenterShift(float shift);
};
