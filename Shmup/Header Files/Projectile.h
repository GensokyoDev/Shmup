#pragma once
#include <SFML\Graphics.hpp>

class Projectile
{
public:
	Projectile(double, double, double, double,  bool);
	~Projectile();
private:
	double X;
	double Y;
	double degreesTurned;
	double horizontalSpeed;
	double verticalSpeed;
	bool collidable;
	sf::Sprite* sprites;
};

