#include "ArtManager.h"
#include <string>
#include <iostream>

ArtManager::ArtManager()
{
}


ArtManager::~ArtManager()
{
}

void ArtManager::loadTexture(std::string) {	
	if (!texture.loadFromFile("Resources/images/playercharacters/2husprites.png"))
	{
		std::cout << "Texture failed to load.";
	}
}

sf::Sprite* ArtManager::getSprite(int x, int y, int x2, int y2) {
	sf::Sprite* sprite = new sf::Sprite();
	sprite->setTextureRect(sf::IntRect(x, y, x2, y2));
	sprite->setTexture(texture);
	sprite->setScale(sf::Vector2f(1.5f, 1.5f));
	return sprite;
}

void ArtManager::setSmooth(bool smooth) {
	texture.setSmooth(smooth);
}