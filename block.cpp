#include "block.h"

Block::Block(float x, float y, float width, float height) {
	setPosition(x, y);
	setSize({ width, height });
	texture.loadFromFile("resources\\blocks.png");
	setTexture(&texture);
	speed = { 0.f, 0.f };
}


OrdinaryBlock::OrdinaryBlock(float x, float y, float width, float height) : Block(x, y, width, height) {
	hasBonus = (std::rand() % 4 != 0);
}

bool OrdinaryBlock::ManageCollision(Score* score) {
	isDestroyed = true;
	score->Increase();
	return false;
}


UnkillableBlock::UnkillableBlock(float x, float y, float width, float height) : Block(x, y, width, height) {
	hasBonus = false;
}

bool UnkillableBlock::ManageCollision(Score* score) {
	return false;
}


BlockWithHealth::BlockWithHealth(float x, float y, float width, float height, unsigned int healthNum, float gap) : Block(x, y, width, height), TEXTURE_GAP(gap) {
	healthMaxNum = healthNum;
	healthCount = healthMaxNum;
	hasBonus = false;
}

bool BlockWithHealth::ManageCollision(Score* score) {
	healthCount--;
	score->Increase();
	if (healthCount == 2 || healthCount == 1) {
		sf::IntRect textureRect = getTextureRect();
		textureRect.top -= static_cast<int>(getSize().y + TEXTURE_GAP);
		setTextureRect(textureRect);
	}
	else if (healthCount == 0)
		isDestroyed = true;
	return false;
}

void BlockWithHealth::RestartLives() {
	healthCount = healthMaxNum;

	sf::IntRect textureRect = getTextureRect();
	textureRect.top = static_cast<int>(getSize().y + TEXTURE_GAP) * (healthCount - 1);
	setTextureRect(textureRect);

	isDestroyed = false;
}


SpeedUpBlock::SpeedUpBlock(float x, float y, float width, float height) : Block(x, y, width, height) {
	hasBonus = false;
}

bool SpeedUpBlock::ManageCollision(Score* score) {
	isDestroyed = true;
	score->Increase();
	return true;
}
