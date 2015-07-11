#pragma once
#include "ControlledCharacter.h"

class Reimu : public ControlledCharacter
{
public:
	Reimu();
	~Reimu();
private:
	std::string* animationFrames; //should just be the images, not strings
};