#include "Collision.h"

struct BoundingCircle
{
	sf::Vector2f centre;
	float radius;
};

sf::Vector2f GetSpriteCenter(const sf::Sprite& object)
{
	auto dimensions = object.getGlobalBounds();

	return sf::Vector2f(dimensions.left + dimensions.width / 2.0f,
		dimensions.top + dimensions.height / 2.0f);
}

sf::Vector2f GetSpriteSize(const sf::Sprite& object)
{
	auto originalSize = object.getTextureRect();
	auto scale = object.getScale();

	return sf::Vector2f(originalSize.width * scale.x, originalSize.height * scale.y);
}

float GetRadiusOfRectangleWithSize(sf::Vector2f size)
{
	return (size.x + size.y) / 4;
}

BoundingCircle GetBoundingCircle(const sf::Sprite& object)
{
	auto objSize = GetSpriteSize(object);
	auto radius = GetRadiusOfRectangleWithSize(objSize);

	return BoundingCircle({ GetSpriteCenter(object), radius });
}

bool AreBoundingCirclesIntersecting(BoundingCircle boundingCircleOne, BoundingCircle boundingCircleTwo)
{
	auto distance = boundingCircleOne.centre - boundingCircleTwo.centre;
	auto magnitudeOfDistanceSquared = distance.x * distance.x + distance.y * distance.y;
	auto maximumCollidingDistanceBetweenBoundings = (boundingCircleOne.radius + boundingCircleTwo.radius) * (boundingCircleOne.radius + boundingCircleTwo.radius);

	return (magnitudeOfDistanceSquared <= maximumCollidingDistanceBetweenBoundings);
}

bool CircleTest(const sf::Sprite& object1, const sf::Sprite& object2)
{
	auto boundingCircleOne = GetBoundingCircle(object1);
	auto boundingCircleTwo = GetBoundingCircle(object2);

	return AreBoundingCirclesIntersecting(boundingCircleOne, boundingCircleTwo);
}