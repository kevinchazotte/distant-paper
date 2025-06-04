#include "UIRenderer.h"

#include "../Utilities/DrawingTypeSerializationConverterUtil.h"

UIRenderer::UIRenderer(sf::RenderWindow& window, sf::Font font) 
	: m_RenderWindow(window), m_Font(font) {}

void UIRenderer::Clear() {
	m_RenderWindow.clear(sf::Color::White);
}

void UIRenderer::Display() {
	m_RenderWindow.display();
}

void UIRenderer::RenderHomeScreen() {
	DrawTitle();
	DrawButton(m_RenderWindow.getSize().x / 2 - kButtonWidth / 2, m_RenderWindow.getSize().y / 2, kButtonWidth, kButtonHeight, "CONNECT", sf::Color::Black);
	DrawButton(m_RenderWindow.getSize().x / 2 - kButtonWidth / 2, m_RenderWindow.getSize().y / 1.6, kButtonWidth, kButtonHeight, "EXIT", sf::Color::Black);
}

void UIRenderer::RenderWhiteboard(const std::unordered_map<std::string, Whiteboard::Types::Drawable>& drawables,
	const Whiteboard::Types::Line& currentLine, const Whiteboard::Types::Rectangle& currentRectangle, const sf::CircleShape& cursorCircle,
	bool isDrawing, WhiteboardStateMachine::DrawTool currentTool) {
	RenderDrawings(drawables, currentLine, currentRectangle, isDrawing, currentTool);
	RenderCursor(cursorCircle, isDrawing, currentTool);
	RenderMenuBar(currentTool);
}

void UIRenderer::RenderDrawings(const std::unordered_map<std::string, Whiteboard::Types::Drawable>& drawables,
	const Whiteboard::Types::Line& currentLine, const Whiteboard::Types::Rectangle& currentRectangle,
	bool isDrawing, WhiteboardStateMachine::DrawTool currentTool) {
	// Draw completed drawable types
	for (const std::pair<std::string, Whiteboard::Types::Drawable>& pair : drawables) {
		if (pair.second.has_line() && pair.second.line().has_start() && pair.second.line().has_end()) {
			DrawLine(DrawingTypeSerializationConverterUtil::ToSFMLVertices(pair.second.line()));
		}
		else if (pair.second.has_rectangle() && pair.second.rectangle().has_start() && pair.second.rectangle().has_end()) {
			DrawRectangle(DrawingTypeSerializationConverterUtil::ToSFMLRectangleShape(pair.second.rectangle()));
		}
	}

	// Draw current line being drawn
	if (isDrawing && currentTool == WhiteboardStateMachine::DrawTool::kMarker && currentLine.has_start() && currentLine.has_end()) {
		DrawLine(DrawingTypeSerializationConverterUtil::ToSFMLVertices(currentLine));
	}

	// Draw current rectangle preview
	if (isDrawing && currentTool == WhiteboardStateMachine::DrawTool::kRectangle && currentRectangle.has_start() && currentRectangle.has_end()) {
		DrawRectangle(DrawingTypeSerializationConverterUtil::ToSFMLRectangleShape(currentRectangle));
	}
}

void UIRenderer::RenderCursor(const sf::CircleShape& cursorCircle, bool isDrawing, WhiteboardStateMachine::DrawTool currentTool) {
	if (isDrawing && currentTool == WhiteboardStateMachine::DrawTool::kEraser) {
		DrawCircle(cursorCircle);
	}
}

void UIRenderer::RenderMenuBar(WhiteboardStateMachine::DrawTool currentTool) {
	sf::RectangleShape menuBar(sf::Vector2f(m_RenderWindow.getSize().x, kMenuBarHeight));
	menuBar.setFillColor(sf::Color::White);
	menuBar.setOutlineColor(sf::Color::Black);
	menuBar.setOutlineThickness(2);
	m_RenderWindow.draw(menuBar);

	// temporary values for visualizing
	int buttonX = 10;
	int buttonY = 10;
	int spacing = kButtonWidth + 10;

	// come up with a better way of distinguishing which tool is currently selected
	sf::Color homeColor = sf::Color::Black;
	sf::Color markerColor = (currentTool == WhiteboardStateMachine::DrawTool::kMarker) ? sf::Color(100, 100, 100) : sf::Color::Black;
	sf::Color rectColor = (currentTool == WhiteboardStateMachine::DrawTool::kRectangle) ? sf::Color(100, 100, 100) : sf::Color::Black;
	sf::Color eraserColor = (currentTool == WhiteboardStateMachine::DrawTool::kEraser) ? sf::Color(100, 100, 100) : sf::Color::Black;

	DrawButton(buttonX, buttonY, kButtonWidth, kButtonHeight, "HOME", homeColor);
	buttonX += spacing;
	DrawButton(buttonX, buttonY, kButtonWidth, kButtonHeight, "MARKER", markerColor);
	buttonX += spacing;
	DrawButton(buttonX, buttonY, kButtonWidth, kButtonHeight, "RECTANGLE", rectColor);
	buttonX += spacing;
	DrawButton(buttonX, buttonY, kButtonWidth, kButtonHeight, "ERASER", eraserColor);
}

void UIRenderer::DrawTitle() {
	sf::Text title(m_Font, "WHITEBOARD");
	title.setCharacterSize(48);
	title.setFillColor(sf::Color::Black);
	title.setPosition(sf::Vector2f(m_RenderWindow.getSize().x / 2 - title.getGlobalBounds().size.x / 2, 200));
	m_RenderWindow.draw(title);
}

void UIRenderer::DrawButton(float x, float y, float width, float height, const std::string& text, sf::Color color) {
	sf::RectangleShape button(sf::Vector2f(width, height));
	button.setPosition(sf::Vector2f(x, y));
	button.setFillColor(sf::Color::White);
	button.setOutlineColor(color);
	button.setOutlineThickness(2);
	m_RenderWindow.draw(button);

	sf::Text buttonText(m_Font, text);
	buttonText.setString(text);
	buttonText.setCharacterSize(16);
	buttonText.setFillColor(color);
	buttonText.setPosition(sf::Vector2f(x + width / 2 - buttonText.getGlobalBounds().size.x / 2,
						  y + height / 2 - buttonText.getGlobalBounds().size.y / 2));
	m_RenderWindow.draw(buttonText);
}

void UIRenderer::DrawLine(const std::array<sf::Vertex, 2>& vertexArray) {
	sf::Vertex vertices[2];
	vertices[0] = vertexArray.at(0);
	vertices[1] = vertexArray.at(1);
	m_RenderWindow.draw(vertices, 2, sf::PrimitiveType::Lines);
}

void UIRenderer::DrawRectangle(const sf::RectangleShape& rectangleShape) {
	m_RenderWindow.draw(rectangleShape);
}

void UIRenderer::DrawCircle(const sf::CircleShape& circleShape) {
	m_RenderWindow.draw(circleShape);
}
