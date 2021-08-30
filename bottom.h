#pragma once
#include "object.h"

class Bottom : public Object {
	const float BOTTOM_WIDTH = 998.f;
	const float BOTTOM_HEIGHT = 19.f;
	const float BOTTOM_X = 1.f;
	const float BOTTOM_Y = 730.f;
	const float ACTIVE_TIME = 8000.f;

	float activeTimeCounter = 0;
public:
	bool isActivated = false;

	Bottom();

	void Activate();
	void ManageActiveTime(float time);
};