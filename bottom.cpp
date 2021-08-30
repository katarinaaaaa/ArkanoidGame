#include "bottom.h"

Bottom::Bottom() {
	setPosition(BOTTOM_X, BOTTOM_Y);
	setSize({ BOTTOM_WIDTH, BOTTOM_HEIGHT });
	texture.loadFromFile("resources\\bottom.png");
	setTexture(&texture);
	speed = { 0.f, 0.f };
}

void Bottom::Activate() {
	isActivated = true;
	activeTimeCounter = 0;
}

void Bottom::ManageActiveTime(float time) {
	if (!isActivated)
		return;
	activeTimeCounter += time;
	if (activeTimeCounter >= ACTIVE_TIME)
		isActivated = false;
}
