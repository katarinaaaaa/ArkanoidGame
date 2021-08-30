#include "object.h"

float Object::GetLeftBound() const {
	return getPosition().x;
}

float Object::GetRightBound() const {
	return getPosition().x + getSize().x;
}

float Object::GetTopBound() const {
	return getPosition().y;
}

float Object::GetBottomBound() const {
	return getPosition().y + getSize().y;
}

sf::Vector2f Object::GetSpeed() const {
	return speed;
}

void Object::SetSpeed(float newSpeedX, float newSpeedY) {
	speed.x = newSpeedX;
	speed.y = newSpeedY;
}

void Object::ReverseXSpeed() {
	speed.x *= -1;
}

void Object::ReverseYSpeed() {
	speed.y *= -1;
}
