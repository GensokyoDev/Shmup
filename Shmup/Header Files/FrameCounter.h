#pragma once
#include <SFML\Graphics.hpp>

class FrameCounter
{
public:
	FrameCounter();
	~FrameCounter();
	double getFPS();
	void restartClock();
	int getFrames();
	void incrementFrames();
private:
	sf::Clock clock;
	int frameCount = 0; //probably should reset safely if above MAX_INT
	double fpsGotten = 1;
};

