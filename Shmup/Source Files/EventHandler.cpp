#include "EventHandler.h"
#include <iostream>
#include <GL/glew.h>

extern bool running;

EventHandler::EventHandler()
{
	keysPressed = new std::vector<int>;
}

EventHandler::~EventHandler()
{
	delete keysPressed;
}

void EventHandler::handleEvents(sf::RenderWindow *window)
{
	sf::Event event;
	while (window->pollEvent(event)) {
		switch (event.type) {

		case sf::Event::Closed:
			std::cout << "closed";
			running = false; // do you really want to close window etc
			break;
		
		case sf::Event::Resized:
			glViewport(0, 0, event.size.width, event.size.height);

		case sf::Event::KeyPressed:
			std::cout << event.key.code << std::endl;
			addKeyIfNotContained(event.key.code);
			break;

		case sf::Event::KeyReleased:
			deleteKeyIfContained(event.key.code);
			break;

		default:
			break;
		}
	}
}

std::vector<int>* EventHandler::getKeys() const
{
	return keysPressed;
}

void EventHandler::addKeyIfNotContained(int key) {
	if (std::find(keysPressed->begin(), keysPressed->end(), key) == keysPressed->end()) {
		keysPressed->push_back(key);
	}
}

void EventHandler::deleteKeyIfContained(int key) {
	if (std::find(keysPressed->begin(), keysPressed->end(), key) != keysPressed->end()) {
		keysPressed->erase(std::remove(keysPressed->begin(), keysPressed->end(), key), keysPressed->end());
	}
}