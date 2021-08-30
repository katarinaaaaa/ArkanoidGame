#pragma once
#include <SFML/Graphics.hpp>

class Object : public sf::RectangleShape {
protected:
	sf::Vector2f speed;
	sf::Texture texture;

public:

	virtual float GetLeftBound() const;
	virtual float GetRightBound() const;
	virtual float GetTopBound() const;
	virtual float GetBottomBound() const;

	sf::Vector2f GetSpeed() const;
	void SetSpeed(float newSpeedX, float newSpeedY);
	void ReverseXSpeed();
	void ReverseYSpeed();
};
