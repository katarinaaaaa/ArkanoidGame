#pragma once
#include <SFML/Graphics.hpp>

class Score : public sf::Text {
	const float SCORE_X = 35.f;
	const float SCORE_Y = 32.f;
	static const int SCORE_INCREMENT = 10;
	static const int SCORE_DECREMENT = 20;

	const std::string scoreTitle = "Score: ";
	sf::Font font;
	int scoreValue = 0;

public:
	Score();

	void Increase();
	void Reduce();
	void Reset();
};
