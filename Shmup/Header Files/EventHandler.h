#pragma once
#include <SFML/Graphics.hpp>

class EventHandler
{
public:
	EventHandler();
	~EventHandler();
	void handleEvents(sf::RenderWindow*);
	std::vector<int>* getKeys() const;
private:
	std::vector<int>* keysPressed;
	void addKeyIfNotContained(int key);
	void deleteKeyIfContained(int key);
};

