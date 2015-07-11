#include "InputHandler.h"
#include "ControlledCharacter.h"
#include <iostream>

static const int GAME = 0;
static const int MENU = 1; // maybe use enum instead, it's now in 2 files.

InputHandler::InputHandler()
{
}

InputHandler::~InputHandler()
{
}

void InputHandler::handleInputs(int gameState, std::vector<int> *keys, ControlledCharacter &controlledCharacter) {
	//maybe just get all booleans out first and then logic
	controlledCharacter.setFocusedMovement(false);
	if ((find(keys->begin(), keys->end(), sf::Keyboard::LShift) != keys->end()) && (gameState == GAME)) {
			controlledCharacter.setFocusedMovement(true);
	}

	if ((find(keys->begin(), keys->end(), sf::Keyboard::Left) != keys->end()) && (gameState == GAME)) {
		controlledCharacter.move(LEFT);
	}
	if ((find(keys->begin(), keys->end(), sf::Keyboard::Right) != keys->end()) && (gameState == GAME)) {
		controlledCharacter.move(RIGHT);
	}
	if ((find(keys->begin(), keys->end(), sf::Keyboard::Up) != keys->end()) && (gameState == GAME)) {
		controlledCharacter.move(UP);
	}
	if ((find(keys->begin(), keys->end(), sf::Keyboard::Down) != keys->end()) && (gameState == GAME)) {
		controlledCharacter.move(DOWN);
	}
}