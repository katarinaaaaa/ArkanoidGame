#pragma once
#include "live.h" 

class LiveBar {
	static const unsigned int LIVES_NUMBER = 4;
	const float LIVE_WIDTH = 40.f;
	const float LIVE_HEIGHT = 36.f;
	const float LIVE_BAR_X = 765.f;
	const float LIVE_BAR_Y = 35.f;
	const float LIVE_BAR_GAP = 15.f;

	std::vector<Live*> lives { LIVES_NUMBER, nullptr };
	int livesCount = LIVES_NUMBER;

public:
	LiveBar();
	~LiveBar();

	bool ProcessFall(); //returns false if lives are over
	void Draw(sf::RenderWindow& window) const;
	void Reset();
};
