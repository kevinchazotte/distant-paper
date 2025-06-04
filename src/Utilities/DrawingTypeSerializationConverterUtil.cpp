#include "DrawingTypeSerializationConverterUtil.h"

sf::Color DrawingTypeSerializationConverterUtil::ToSFMLColor(uint32_t color) {
	uint8_t a = color & 0xFF;
	uint8_t b = (color >> 8) & 0xFF;
	uint8_t g = (color >> 16) & 0xFF;
	uint8_t r = (color >> 24) & 0xFF;
	return sf::Color(r, g, b, a);
}

uint32_t DrawingTypeSerializationConverterUtil::ToColor(const sf::Color& color) {
	return (static_cast<uint32_t>(color.r) << 24) | (static_cast<uint32_t>(color.g) << 16) | 
		(static_cast<uint32_t>(color.b) << 8) | (static_cast<uint32_t>(color.a));
}

sf::Vertex DrawingTypeSerializationConverterUtil::ToSFMLVertex(const Whiteboard::Types::Point& point) {
	sf::Vertex vertex;
	vertex.position = sf::Vector2f(point.x(), point.y());
	vertex.color = ToSFMLColor(point.color());
	return vertex;
}

sf::Vertex DrawingTypeSerializationConverterUtil::ToSFMLVertex(float x, float y, const sf::Color& color) {
	sf::Vertex vertex;
	vertex.position = sf::Vector2f(x, y);
	vertex.color = color;
	return vertex;
}

Whiteboard::Types::Point DrawingTypeSerializationConverterUtil::ToPoint(const sf::Vertex& vertex) {
	Whiteboard::Types::Point point;
	point.set_x(vertex.position.x);
	point.set_y(vertex.position.y);
	point.set_color(ToColor(vertex.color));
	return point;
}


Whiteboard::Types::Point DrawingTypeSerializationConverterUtil::ToPoint(const sf::Vector2f& position, const sf::Color& color) {
	Whiteboard::Types::Point point;
	point.set_x(position.x);
	point.set_y(position.y);
	point.set_color(ToColor(color));
	return point;
}

Whiteboard::Types::Point DrawingTypeSerializationConverterUtil::ToPoint(float x, float y, const sf::Color& color) {
	Whiteboard::Types::Point point;
	point.set_x(x);
	point.set_y(y);
	point.set_color(ToColor(color));
	return point;
}

std::array<sf::Vertex, 2> DrawingTypeSerializationConverterUtil::ToSFMLVertices(const Whiteboard::Types::Line& line) {
	return { ToSFMLVertex(line.start()), ToSFMLVertex(line.end()) };
}

Whiteboard::Types::Line DrawingTypeSerializationConverterUtil::ToLine(const sf::Vertex& start, const sf::Vertex& end, const std::string& id) {
	Whiteboard::Types::Line line;
	line.set_id(id);
	Whiteboard::Types::Point* startPoint = line.mutable_start();
	*startPoint = ToPoint(start);
	Whiteboard::Types::Point* endPoint = line.mutable_end();
	*endPoint = ToPoint(end);
	return line;
}

Whiteboard::Types::Line DrawingTypeSerializationConverterUtil::ToLine(const sf::Vector2f& startPosition, const sf::Vector2f& endPosition,
	const sf::Color& color, const std::string& id) {
	Whiteboard::Types::Line line;
	line.set_id(id);
	Whiteboard::Types::Point* start = line.mutable_start();
	*start = ToPoint(startPosition, color);
	Whiteboard::Types::Point* end = line.mutable_end();
	*end = ToPoint(endPosition, color);
	return line;
}

sf::RectangleShape DrawingTypeSerializationConverterUtil::ToSFMLRectangleShape(const Whiteboard::Types::Rectangle& rectangle) {
	sf::Vertex start = ToSFMLVertex(rectangle.start());
	sf::Vertex end = ToSFMLVertex(rectangle.end());
	float minX = std::min(start.position.x, end.position.x);
	float minY = std::min(start.position.y, end.position.y);
	float width = std::abs(start.position.x - end.position.x);
	float height = std::abs(start.position.y - end.position.y);
	sf::RectangleShape rectangleShape(sf::Vector2f(width, height));
	rectangleShape.setPosition(sf::Vector2f(minX, minY));
	rectangleShape.setOutlineColor(start.color);
	rectangleShape.setOutlineThickness(1);
	rectangleShape.setFillColor(sf::Color::Transparent);
	return rectangleShape;
}

Whiteboard::Types::Rectangle DrawingTypeSerializationConverterUtil::ToRectangle(const sf::RectangleShape& rectangleShape, const std::string& id) {
	Whiteboard::Types::Rectangle rectangle;
	sf::Vector2f start = rectangleShape.getPosition();
	sf::Vector2f end = start + rectangleShape.getSize();
	Whiteboard::Types::Point* startPoint = rectangle.mutable_start();
	*startPoint = ToPoint(start, rectangleShape.getOutlineColor());
	Whiteboard::Types::Point* endPoint = rectangle.mutable_end();
	*endPoint = ToPoint(end, rectangleShape.getOutlineColor());
	return rectangle;
}

Whiteboard::Types::Rectangle DrawingTypeSerializationConverterUtil::ToRectangle(const sf::Vertex& start, const sf::Vertex& end, const std::string& id) {
	Whiteboard::Types::Rectangle rectangle;
	Whiteboard::Types::Point* startPoint = rectangle.mutable_start();
	*startPoint = ToPoint(start);
	Whiteboard::Types::Point* endPoint = rectangle.mutable_end();
	*endPoint = ToPoint(end);
	return rectangle;
}

Whiteboard::Types::Rectangle DrawingTypeSerializationConverterUtil::ToRectangle(const sf::Vector2f& startPosition, const sf::Vector2f& endPosition,
	const sf::Color& color, const std::string& id) {
	Whiteboard::Types::Rectangle rectangle;
	rectangle.set_id(id);
	Whiteboard::Types::Point* start = rectangle.mutable_start();
	*start = ToPoint(startPosition, color);
	Whiteboard::Types::Point* end = rectangle.mutable_end();
	*end = ToPoint(endPosition, color);
	return rectangle;
}
