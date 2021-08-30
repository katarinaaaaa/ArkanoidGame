#include "score.h"

Score::Score() {
	font.loadFromFile("resources\\dolcevitabold.ttf");
	setFont(font);
	setCharacterSize(39);
	setFillColor(sf::Color::White);
	setString(scoreTitle + std::to_string(scoreValue));
	setPosition({ SCORE_X , SCORE_Y });
}

void Score::Increase() {
	scoreValue += SCORE_INCREMENT;
	setString(scoreTitle + std::to_string(scoreValue));
}

void Score::Reduce() {
	scoreValue -= SCORE_DECREMENT;
	setString(scoreTitle + std::to_string(scoreValue));
}

void Score::Reset() {
	scoreValue = 0;
	setString(scoreTitle + std::to_string(scoreValue));
}
