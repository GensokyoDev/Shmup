#pragma once
#include <vector>
#include "ControlledCharacter.h"

enum Input { LEFT, RIGHT, UP, DOWN };

class InputHandler
{
public:
	InputHandler();
	~InputHandler();
	void handleInputs(int gameState, std::vector<int> *keys, ControlledCharacter& controlledCharacter);
};