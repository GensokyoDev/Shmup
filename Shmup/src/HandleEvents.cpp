#include "HandleEvents.h"
#include <SFML/Graphics.hpp>
#include <iostream>

std::vector<int> keysPressed;

HandleEvents::HandleEvents(sf::RenderWindow &window, int state)
{
	keysPressed.empty();

	bool isPressedLeft = false;
	bool isPressedRight = false;
	bool isPressedUp = false;
	bool isPressedDown = false;
	bool isPressedShift = false; // might only check with movement
	bool isPressedLCtrl = false;
	bool isPressedZ = false;
	bool isPressedX = false;
	bool isPressedEnter = false;
	bool isPressedEscape = false;

	sf::Event event;
	while (window.pollEvent(event)) {
		switch (event.type) {

		case sf::Event::Closed:
			std::cout << "closed";
			window.close(); // do you really want to close window etc
			break;

		case sf::Event::KeyPressed:
			switch (event.key.code) {
			case sf::Keyboard::Left:
				isPressedLeft = true;
				break;
			case sf::Keyboard::Right:
				isPressedRight = true;
				break;
			case sf::Keyboard::Up:
				isPressedUp = true;
				break;
			case sf::Keyboard::Down:
				isPressedDown = true;
				break;
			case sf::Keyboard::LControl:
				isPressedLCtrl = true;
				break;
			case sf::Keyboard::Z:
				isPressedZ = true;
				break;
			case sf::Keyboard::X:
				isPressedX = true;
				break;
			case sf::Keyboard::Return:
				isPressedEnter = true;
				break;
			case sf::Keyboard::Escape:
				isPressedEscape = true;
				std::cout << "the escape key was pressed" << std::endl;
				std::cout << "escape:" << event.key.shift << std::endl;  //nec for focused movement? or just test for shift key
				break;
			default:
				break;
			}
			break;

		case sf::Event::KeyReleased:
			switch (event.key.code) {	
			case sf::Keyboard::Left:
				isPressedLeft = false;
				break;
			case sf::Keyboard::Right:
				isPressedRight = false;
				break;
			case sf::Keyboard::Up:
				isPressedUp = false;
				break;
			case sf::Keyboard::Down:
				isPressedDown = false;
				break;
			case sf::Keyboard::LControl:
				isPressedLCtrl = false;
				break;
			case sf::Keyboard::Z:
				isPressedZ = false;
				break;
			case sf::Keyboard::X:
				isPressedX = false;
				break;
			case sf::Keyboard::Return:
				isPressedEnter = false;
				break;
			case sf::Keyboard::Escape:
				isPressedEscape = false;
				break;
			default:
				break;
			}
			break;

		default:
			break;
		}
	}
}


HandleEvents::~HandleEvents()
{
}
