#include <SFML/Graphics.hpp>
#include <iostream>
#include "HandleEvents.h"

static const int fps = 60;

//split source and header files in directory
int main()
{
	// Make a setup function for all this and more
	sf::RenderWindow window(sf::VideoMode(1024, 768), "muh touhou clone");
	window.setFramerateLimit(fps);
	window.setKeyRepeatEnabled(false);
	int state = 0; //define states

	//circle for test
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	// touhou sprite test
	sf::Texture texture;
	texture.setSmooth(true);
	//texture.setSmooth(true);
	if (!texture.loadFromFile("Resources/images/playercharacters/2husprites.png"))
	{
		// error...
	}
	sf::Sprite sprite;
	sprite.setTextureRect(sf::IntRect(0, 0, 26, 44));
	sprite.setTexture(texture);
	sprite.setScale(sf::Vector2f(1.5f, 1.5f));
	sprite.setPosition(200, 200);

	int frameCounter = 0;

	sf::Clock clock;
	float fpsGotten = 1;

	while (window.isOpen()) {	

		// Frames Per Second
		if (frameCounter % fps == 0) {
			sf::Time currentTime = clock.getElapsedTime();
			fpsGotten = 1.f / (currentTime.asSeconds()/fps);
			std::cout << "current fps = " << fpsGotten << " Intended fps = " << fps << std::endl;
			clock.restart();
		}
		frameCounter++;
		

		HandleEvents eventHandler(window, state);
		

		window.clear(sf::Color::Black);
		window.draw(shape);
		window.draw(sprite);
		window.display();
	}
	return 0;
}