#include "Window.h"
#include <iostream>

Window::Window(int width, int height, char *title) 
{
	m_Width = width; 
	m_Height = height;
	m_Title = title;
	m_Open = true;
}

Window::~Window()
{
}

bool Window::init()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 0;
	m_Window = new sf::RenderWindow(sf::VideoMode(1024, 768), "muh touhou clone", sf::Style::Default, settings);
	if (!m_Window)
	{
		std::cout << "Failed to create window." << std::endl;
		return false;
	}
	return true;
}

bool Window::closed()
{
	return m_Open;
}

bool Window::draw(sf::Sprite &characterSprite)
{
	m_Window->draw(characterSprite);
	return true;
}

bool Window::display()
{
	m_Window->display();
	return true;
}

bool Window::clear()
{
	m_Window->clear(sf::Color::Black);
	return true;
}

bool Window::setFramerateLimit(int fps)
{
	if (m_Window != NULL) 
	{
		m_Window->setFramerateLimit(fps);
		return true;
	}
	else {
		return false;
	}
}

bool Window::setKeyRepeatEnabled(bool keyRepeat)
{
	if (m_Window != NULL) 
	{
		m_Window->setKeyRepeatEnabled(keyRepeat);
		return true;
	}
	else {
		return false;
	}
}

sf::RenderWindow* Window::getWindow() {
	return m_Window;
}