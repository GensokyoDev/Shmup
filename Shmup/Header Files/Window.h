#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Window
{
public:
	Window(int width, int height, char *title);
	~Window();
	bool init();
	bool closed();
	bool update();
	bool draw(sf::Sprite &characterSprite);
	bool display();
	bool clear();
	bool setFramerateLimit(int);
	bool setKeyRepeatEnabled(bool);
	sf::RenderWindow* getWindow();
private:
	sf::RenderWindow *m_Window;
	int m_Width;
	int m_Height;
	char *m_Title;
	bool m_Open = false;
};

