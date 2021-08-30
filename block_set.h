#pragma once
#include "block.h"

class BlockSet {
	static const unsigned int BLOCKS_ROW_NUM = 11;
	static const unsigned int BLOCKS_COLUMN_NUM = 7;
	static const unsigned int UNKILLABLE_BLOCKS_NUM = 12;
	const float BLOCK_WIDTH = 78.f;
	const float BLOCK_HEIGHT = 36.f;
	const float BLOCK_SET_X = 60.f;
	const float BLOCK_SET_Y = 95.f;
	const float BLOCK_SET_GAP = 2.f;
	const float BLOCK_TEXTURE_GAP = 5.f;
	const float MOVING_BLOCK_SPEED = 0.05f;

	unsigned int blockCounter = BLOCKS_ROW_NUM * BLOCKS_COLUMN_NUM - UNKILLABLE_BLOCKS_NUM;

	void FillSet();
	void FreeSet();
	float GetSetBottom();

public:
	std::vector<std::vector<Block*>> set{ BLOCKS_ROW_NUM, std::vector<Block*> { BLOCKS_COLUMN_NUM, nullptr } };

	BlockWithHealth* movingBlock;
	bool isMovingBlockActivated = false;

	BlockSet();
	~BlockSet();

	void RebuildSet();
	void Draw(sf::RenderWindow& window) const;
	sf::FloatRect GetRectangle() const; //for checking intersections
	sf::Vector2f GetBlocksNumber() const;
	void ReduceBlockNumber();
	bool IsBlocksAreOver();

	void ActivateMovingBlock();
	void UpdateMovingBlockPosition(float time);

private:
	//template for block set pattern
	unsigned int setTemplate[BLOCKS_ROW_NUM][BLOCKS_COLUMN_NUM] = {
		{ 11, 1, 3, 22, 2, 2, 2 },
		{ 11, 11, 1, 3, 22, 2, 2 },
		{ 11, 11, 11, 1, 3, 22, 2 },
		{ 22, 5, 11, 4, 1, 3, 22 },
		{ 22, 5, 5, 5, 4, 1, 3 },
		{ 22, 5, 5, 5, 5, 4, 1 },
		{ 22, 5, 5, 5, 4, 1, 3 },
		{ 22, 5, 11, 4, 1, 3, 22 },
		{ 11, 11, 11, 1, 3, 22, 2 },
		{ 11, 11, 1, 3, 22, 2, 2 },
		{ 11, 1, 3, 22, 2, 2, 2 }
    };
};
