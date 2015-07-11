#pragma once

#include <string>
#include <SFML\Graphics.hpp>

class ControlledCharacter
{
public:
	ControlledCharacter();
	virtual ~ControlledCharacter();
	void move(int);
	void setFocusedMovement(bool focus);
	bool getFocusedMovement() const { return focusedMovement; }
	void setSprite(sf::Sprite*);
	bool isSpriteSet();
	void setPosition(float x, float y);
	double getX() const { return m_X; }
	void setX(double x) { m_X = x; }
	double getY() const { return m_Y; }
	void setY(double y) { m_Y = y; }
private:
	bool focusedMovement = false;
	float focusedMultiplier;
	int character;
	std::string* animationFrames;
	sf::Sprite* characterSprite;
	bool spriteSet = false;
protected:
	float baseSpeed;
	double m_X;
	double m_Y;
};