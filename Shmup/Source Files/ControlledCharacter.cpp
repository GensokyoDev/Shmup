#include "ControlledCharacter.h"
#include "InputHandler.h"
#include <iostream>

ControlledCharacter::ControlledCharacter()
{
}

ControlledCharacter::~ControlledCharacter()
{
}

void ControlledCharacter::move(int direction) {
	if (focusedMovement) {
		focusedMultiplier = 0.5;
	}
	else {
		focusedMultiplier = 1.0;
	}
	switch (direction) {
	case UP:
		m_Y -= baseSpeed * focusedMultiplier;
		characterSprite->move(0, -baseSpeed * focusedMultiplier);
		break;
	case DOWN:
		m_Y += baseSpeed * focusedMultiplier;
		characterSprite->move(0, baseSpeed * focusedMultiplier);
		break;
	case LEFT:
		m_X -= baseSpeed * focusedMultiplier;
		characterSprite->move(-baseSpeed * focusedMultiplier, 0);
		break;
	case RIGHT:
		m_X += baseSpeed * focusedMultiplier;
		characterSprite->move(baseSpeed * focusedMultiplier, 0);
		break;
	default:
		break;
	}
}

void ControlledCharacter::setFocusedMovement(bool focus) {
	focusedMovement = focus;
}

void ControlledCharacter::setSprite(sf::Sprite* sprite) {
	characterSprite = sprite;
	spriteSet = true;
}

void ControlledCharacter::setPosition(float x, float y) {
	m_X = x;
	m_Y = y;
	characterSprite->setPosition(x, y);
}

bool ControlledCharacter::isSpriteSet() {
	return spriteSet;
}