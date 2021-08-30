#include "ball.h"

Ball::Ball(float windowWidth, float paddleY, float shift) : paddleCenterShift(shift) {
	isOnPaddle = true;
	setSize({ BALL_SIZE, BALL_SIZE });
	setPosition((static_cast<float>(windowWidth) - BALL_SIZE) / 2 + paddleCenterShift, paddleY - BALL_SIZE);
	texture.loadFromFile("resources\\ball.png");
	setTexture(&texture);
	speed = DEFAULT_SPEED;
}

void Ball::FollowPaddle(const Paddle* paddle) {
	setPosition({ (paddle->getPosition().x + (paddle->getSize().x - getSize().x) / 2) + paddleCenterShift, paddle->getPosition().y - getSize().y });
}

void Ball::UpdatePosition(const Paddle* paddle, float time) {
	if (isOnPaddle)
		FollowPaddle(paddle);
	else
		move(speed.x * time, speed.y * time);
}

bool Ball::IsFall(int windowHeight) const {
	return GetTopBound() > windowHeight;
}

void Ball::ChangeSpeedAfterPaddleColision(float coefficient) {
	sf::Vector2f oldSpeed = GetSpeed();
	speed.x = DEPARTURE_SPEED.x * coefficient;
	speed = speed / sqrt(speed.x * speed.x + speed.y * speed.y) * sqrt(oldSpeed.x * oldSpeed.x + oldSpeed.y * oldSpeed.y);
}

void Ball::SetDepartureSpeedToDefault() {
	DEPARTURE_SPEED = DEFAULT_SPEED;
}

void Ball::SetDepartureSpeed() {
	speed = DEPARTURE_SPEED;
}

void Ball::SpeedUp() {
	speed *= SPEED_UP_COEF;
	DEPARTURE_SPEED *= SPEED_UP_COEF;
}

void Ball::SlowDown() {
	speed *= SLOW_DOWN_COEF;
	DEPARTURE_SPEED *= SLOW_DOWN_COEF;
}

void Ball::SetPaddleCenterShift(float shift) {
	paddleCenterShift = shift;
}
