#pragma once
#include <string>
#include <SFML\Graphics.hpp>

class ArtManager
{
public:
	ArtManager();
	~ArtManager();
	void loadTexture(std::string);
	sf::Sprite* getSprite(int, int, int, int);
	bool getSmooth();
	void setSmooth(bool);
private:
	bool smooth = true;
	sf::Texture texture;
};

