#include "UIRenderer.h"

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

void UIRenderer::RenderWhiteboard(const std::vector<WhiteboardStateMachine::Line>& lines, const std::vector<WhiteboardStateMachine::Rectangle>& rectangles,
								const WhiteboardStateMachine::Line& currentLine, const WhiteboardStateMachine::Rectangle& currentRect,
								bool isDrawing, bool isRectStarted, WhiteboardStateMachine::DrawTool currentTool) {
	RenderMenuBar(currentTool);
	RenderDrawings(lines, rectangles, currentLine, currentRect, isDrawing, isRectStarted, currentTool);
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

void UIRenderer::RenderDrawings(const std::vector<WhiteboardStateMachine::Line>& lines, const std::vector<WhiteboardStateMachine::Rectangle>& rectangles,
							  const WhiteboardStateMachine::Line& currentLine, const WhiteboardStateMachine::Rectangle& currentRect,
							  bool isDrawing, bool isRectStarted, WhiteboardStateMachine::DrawTool currentTool) {
	// Draw completed lines
	for (const auto& line : lines) {
		if (line.points.size() < 2) continue;
		for (size_t i = 1; i < line.points.size(); ++i) {
			DrawLine(line.points[i-1].position, line.points[i].position, line.points[i].color);
		}
	}

	// Draw current line being drawn
	if (isDrawing && currentTool == WhiteboardStateMachine::DrawTool::kMarker && currentLine.points.size() >= 2) {
		for (size_t i = 1; i < currentLine.points.size(); ++i) {
			DrawLine(currentLine.points[i-1].position, currentLine.points[i].position, currentLine.points[i].color);
		}
	}

	// Draw completed rectangles
	for (const auto& rect : rectangles) {
		if (rect.isComplete) {
			DrawRectangle(rect.start, rect.end, sf::Color::Black);
		}
	}

	// Draw current rectangle preview
	if (isRectStarted && currentTool == WhiteboardStateMachine::DrawTool::kRectangle) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(m_RenderWindow);
		DrawRectangle(currentRect.start, sf::Vector2f(mousePos.x, mousePos.y), sf::Color(128, 128, 128));
	}
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

void UIRenderer::DrawLine(sf::Vector2f start, sf::Vector2f end, sf::Color color) {
	sf::Vertex startVertex;
	startVertex.position = start;
	startVertex.color = color;

	sf::Vertex endVertex;
	endVertex.position = start;
	endVertex.color = color;
	sf::Vertex line[2] = { startVertex, endVertex };
	m_RenderWindow.draw(line, 2, sf::PrimitiveType::Lines);
}

void UIRenderer::DrawRectangle(sf::Vector2f start, sf::Vector2f end, sf::Color color) {
	sf::RectangleShape rect;
	rect.setPosition(sf::Vector2f(std::min(start.x, end.x), std::min(start.y, end.y)));
	rect.setSize(sf::Vector2f(std::abs(end.x - start.x), std::abs(end.y - start.y)));
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(color);
	rect.setOutlineThickness(2);
	m_RenderWindow.draw(rect);
}
