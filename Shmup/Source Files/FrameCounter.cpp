#include "FrameCounter.h"
#include <iostream>

extern const int fps;

FrameCounter::FrameCounter()
{
}


FrameCounter::~FrameCounter()
{
}

double FrameCounter::getFPS() {
	sf::Time currentTime = clock.getElapsedTime();
	fpsGotten = 1.f / (currentTime.asSeconds() / fps);
	return fpsGotten;
}

void FrameCounter::restartClock() {
	clock.restart();
}

int FrameCounter::getFrames() {
	return this->frameCount;
}

void FrameCounter::incrementFrames() {
	frameCount++;
}