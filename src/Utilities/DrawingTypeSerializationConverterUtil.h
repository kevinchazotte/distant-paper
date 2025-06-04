#pragma once

#include <array>

#include <SFML/Graphics.hpp>

#include "drawing_types.grpc.pb.h"
#include "drawing_types.pb.h"

class DrawingTypeSerializationConverterUtil {
public:

	// Protobuf / SFML color conversion
	static sf::Color ToSFMLColor(uint32_t color);
	static uint32_t ToColor(const sf::Color& color);

	// Protobuf to SFML vertex conversion
	static sf::Vertex ToSFMLVertex(const Whiteboard::Types::Point& point);
	static sf::Vertex ToSFMLVertex(float x, float y, const sf::Color& color);

	// SFML to Protobuf point conversion
	static Whiteboard::Types::Point ToPoint(const sf::Vertex& vertex);
	static Whiteboard::Types::Point ToPoint(const sf::Vector2f& position, const sf::Color& color);
	static Whiteboard::Types::Point ToPoint(float x, float y, const sf::Color& color);

	// Line conversions
	static std::array<sf::Vertex, 2> ToSFMLVertices(const Whiteboard::Types::Line& line);
	static Whiteboard::Types::Line ToLine(const sf::Vertex& start, const sf::Vertex& end, const std::string& id);
	static Whiteboard::Types::Line ToLine(const sf::Vector2f& startPosition, const sf::Vector2f& endPosition,
		const sf::Color& color, const std::string& id);

	// Rectangle conversions
	static sf::RectangleShape ToSFMLRectangleShape(const Whiteboard::Types::Rectangle& rectangle);
	static Whiteboard::Types::Rectangle ToRectangle(const sf::RectangleShape& rectangleShape, const std::string& id);
	static Whiteboard::Types::Rectangle ToRectangle(const sf::Vertex& start, const sf::Vertex& end, const std::string& id);
	static Whiteboard::Types::Rectangle ToRectangle(const sf::Vector2f& startPosition, const sf::Vector2f& endPosition,
		const sf::Color& color, const std::string& id);
};
