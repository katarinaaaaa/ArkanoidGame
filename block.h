#pragma once
#include "object.h"
#include "score.h"

class Block : public Object {
public:
	bool isDestroyed = false;
	bool hasBonus = false;

	Block(float x, float y, float width, float height);
	virtual bool ManageCollision(Score* score) = 0;
};


class OrdinaryBlock : public Block {
public:
	OrdinaryBlock(float x, float y, float width, float height);
	virtual bool ManageCollision(Score* score) override;
};


class UnkillableBlock : public Block {
public:
	UnkillableBlock(float x, float y, float width, float height);
	virtual bool ManageCollision(Score* score) override;
};


class BlockWithHealth : public Block {
	const float TEXTURE_GAP;
	unsigned int healthMaxNum;
	unsigned int healthCount;

public:
	BlockWithHealth(float x, float y, float width, float height, unsigned int healthNum, float gap);
	virtual bool ManageCollision(Score* score) override;
	void RestartLives(); //to restart bonus moving block
};


class SpeedUpBlock : public Block {
public:
	SpeedUpBlock(float x, float y, float width, float height);
	virtual bool ManageCollision(Score* score) override;
};
