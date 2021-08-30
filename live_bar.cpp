#include "live_bar.h"

LiveBar::LiveBar() {
	for (int i = 0; i < LIVES_NUMBER; ++i)
		lives[i] = new Live(LIVE_BAR_X + i * (LIVE_BAR_GAP + LIVE_WIDTH), LIVE_BAR_Y, LIVE_WIDTH, LIVE_HEIGHT);
}

LiveBar::~LiveBar() {
	for (auto& live : lives)
		delete live;
}

bool LiveBar::ProcessFall() {
	livesCount--;
	lives[livesCount]->DeleteLive(LIVE_WIDTH, LIVE_HEIGHT);
	if (livesCount == 0)
		return false;
	return true;
}

void LiveBar::Draw(sf::RenderWindow& window) const {
	for (auto& live : lives)
		window.draw(*live);
}

void LiveBar::Reset() {
	for (auto& live : lives)
		live->ReturnLive(LIVE_WIDTH, LIVE_HEIGHT);
	livesCount = LIVES_NUMBER;
}
