#include "bonus.h"

Bonus::Bonus() {
	setPosition(0.f, 0.f);
	setSize({ BONUS_WIDTH, BONUS_HEIGHT });
	texture.loadFromFile("resources\\bonuses.png");
	setTexture(&texture);
	speed = { 0.f, BONUS_MOVE_SPEED };
}

void Bonus::SetNewRandomType() {
	type = static_cast<BONUS_TYPE>(std::rand() % static_cast<int>(BONUS_TYPE::TYPES_NUM));
	setTextureRect(sf::IntRect(static_cast<int>(BONUS_WIDTH + BONUS_TEXTURE_GAP) * static_cast<int>(type), 0, static_cast<int>(BONUS_WIDTH), static_cast<int>(BONUS_HEIGHT)));
}

void Bonus::GetBonus(const sf::Vector2f& blockPosition, const sf::Vector2f& blockSize) {
	SetNewRandomType();
	isOnGameField = true;
	setPosition(blockPosition.x + (blockSize.x - BONUS_WIDTH) / 2, blockPosition.y + blockSize.y / 2);
}

bool Bonus::UpdatePosition(const Paddle* paddle, int windowHeight, float time) {
	move(speed * time);
	if (GetTopBound() > windowHeight)
		isOnGameField = false;
	if (getGlobalBounds().intersects(paddle->getGlobalBounds())) {
		isOnGameField = false;
		return true;
	}
	return false;
}

BONUS_TYPE Bonus::GetBonusType() {
	return type;
}
