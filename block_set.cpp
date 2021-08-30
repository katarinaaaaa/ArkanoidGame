#pragma once
#include "block_set.h"

BlockSet::BlockSet() {
	FillSet();
}

BlockSet::~BlockSet() {
	FreeSet();
}

void BlockSet::FillSet() {
	srand(static_cast<unsigned int>(time(nullptr)));
	float x = BLOCK_SET_X;
	for (int i = 0; i < BLOCKS_ROW_NUM; ++i) {
		float y = BLOCK_SET_Y;
		for (int j = 0; j < BLOCKS_COLUMN_NUM; ++j) {
			Block* block;
			if (setTemplate[i][j] == 1 || setTemplate[i][j] == 2 || setTemplate[i][j] == 3) {
				block = new OrdinaryBlock(x, y, BLOCK_WIDTH, BLOCK_HEIGHT);
				int textureX = (setTemplate[i][j] - 1) * static_cast<int>(BLOCK_WIDTH + BLOCK_TEXTURE_GAP);
				block->setTextureRect(sf::IntRect(textureX, 0, static_cast<int>(BLOCK_WIDTH), static_cast<int>(BLOCK_HEIGHT)));
			}
			else if (setTemplate[i][j] == 4) {
				block = new SpeedUpBlock(x, y, BLOCK_WIDTH, BLOCK_HEIGHT);
				block->setTextureRect(sf::IntRect(static_cast<int>(BLOCK_WIDTH + BLOCK_TEXTURE_GAP) * 3, 0, static_cast<int>(BLOCK_WIDTH), static_cast<int>(BLOCK_HEIGHT)));
			}
			else if (setTemplate[i][j] == 5) {
				block = new UnkillableBlock(x, y, BLOCK_WIDTH, BLOCK_HEIGHT);
				block->setTextureRect(sf::IntRect(static_cast<int>(BLOCK_WIDTH + BLOCK_TEXTURE_GAP) * 4, 0, static_cast<int>(BLOCK_WIDTH), static_cast<int>(BLOCK_HEIGHT)));
			}
			else if (setTemplate[i][j] == 11) {
				block = new BlockWithHealth(x, y, BLOCK_WIDTH, BLOCK_HEIGHT, 3, BLOCK_TEXTURE_GAP);
				block->setTextureRect(sf::IntRect(0, static_cast<int>(BLOCK_HEIGHT + BLOCK_TEXTURE_GAP) * 2, static_cast<int>(BLOCK_WIDTH), static_cast<int>(BLOCK_HEIGHT)));
			}
			else if (setTemplate[i][j] == 22) {
				block = new BlockWithHealth(x, y, BLOCK_WIDTH, BLOCK_HEIGHT, 2, BLOCK_TEXTURE_GAP);
				block->setTextureRect(sf::IntRect(static_cast<int>(BLOCK_WIDTH + BLOCK_TEXTURE_GAP), static_cast<int>(BLOCK_HEIGHT + BLOCK_TEXTURE_GAP), static_cast<int>(BLOCK_WIDTH), static_cast<int>(BLOCK_HEIGHT)));
			}
			else
				block = nullptr;
			set[i][j] = block;
			y += BLOCK_HEIGHT + BLOCK_SET_GAP;
		}
		x += BLOCK_WIDTH + BLOCK_SET_GAP;
	}

	movingBlock = new BlockWithHealth(0.f, 0.f, BLOCK_WIDTH, BLOCK_HEIGHT, 3, BLOCK_TEXTURE_GAP);
	movingBlock->setTextureRect(sf::IntRect(0, static_cast<int>(BLOCK_HEIGHT + BLOCK_TEXTURE_GAP) * 2, static_cast<int>(BLOCK_WIDTH), static_cast<int>(BLOCK_HEIGHT)));
}

void BlockSet::FreeSet() {
	for (int i = 0; i < BLOCKS_ROW_NUM; ++i) {
		for (int j = 0; j < BLOCKS_COLUMN_NUM; ++j)
			delete set[i][j];
	}
	delete movingBlock;
}

void BlockSet::RebuildSet() {
	FreeSet();
	FillSet();
	blockCounter = BLOCKS_ROW_NUM * BLOCKS_COLUMN_NUM - UNKILLABLE_BLOCKS_NUM;
}

void BlockSet::Draw(sf::RenderWindow& window) const {
	for (int i = 0; i < BLOCKS_ROW_NUM; ++i)
		for (int j = 0; j < BLOCKS_COLUMN_NUM; ++j)
			if (!set[i][j]->isDestroyed)
				window.draw(*set[i][j]);
	if (isMovingBlockActivated && !movingBlock->isDestroyed)
		window.draw(*movingBlock);
}

sf::FloatRect BlockSet::GetRectangle() const {
	float width = (BLOCK_WIDTH + BLOCK_SET_GAP) * BLOCKS_ROW_NUM - BLOCK_SET_GAP;
	float height = (BLOCK_HEIGHT + BLOCK_SET_GAP) * BLOCKS_COLUMN_NUM - BLOCK_SET_GAP;
	return sf::FloatRect{ BLOCK_SET_X, BLOCK_SET_Y, width, height };
}

sf::Vector2f BlockSet::GetBlocksNumber() const {
	return { BLOCKS_ROW_NUM , BLOCKS_COLUMN_NUM };
}

void BlockSet::ReduceBlockNumber() {
	blockCounter--;
}

bool BlockSet::IsBlocksAreOver() {
	return blockCounter == 0;
}

float BlockSet::GetSetBottom() {
	for (int j = BLOCKS_COLUMN_NUM - 1; j >= 0; --j)
		for (int i = 0; i < BLOCKS_ROW_NUM; ++i)
			if (!set[i][j]->isDestroyed)
				return BLOCK_SET_Y + (j + 1) * (BLOCK_HEIGHT + BLOCK_SET_GAP);
	return BLOCK_SET_Y;
}

void BlockSet::ActivateMovingBlock() {
	if (isMovingBlockActivated)
		return;
	isMovingBlockActivated = true;
	movingBlock->RestartLives();
	movingBlock->setPosition({ BLOCK_SET_X , GetSetBottom() });
	movingBlock->SetSpeed(MOVING_BLOCK_SPEED, 0.f);
	blockCounter++;
}

void BlockSet::UpdateMovingBlockPosition(float time) {
	if (!isMovingBlockActivated || movingBlock->isDestroyed)
		return;
	movingBlock->move(movingBlock->GetSpeed().x * time, 0.f);
	if (movingBlock->GetLeftBound() <= BLOCK_SET_X || movingBlock->GetRightBound() >= BLOCK_SET_X + GetRectangle().width)
		movingBlock->ReverseXSpeed();
}
