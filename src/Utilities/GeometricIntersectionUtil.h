#pragma once

#include <array>

#include <SFML/Graphics.hpp>

class GeometricIntersectionUtil {
public:

	static bool CircleIntersectsLineSegment(std::array<sf::Vertex, 2> vertices, sf::Vector2f center, float radius);
	static bool CircleIntersectsLineSegment(sf::Vector2f start, sf::Vector2f end, sf::Vector2f center, float radius);
	static bool CircleIntersectsRectangle(sf::RectangleShape rectangleShape, sf::Vector2f center, float radius);
};
