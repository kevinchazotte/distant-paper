#include "UIRenderer.h"

UIRenderer::UIRenderer(sf::RenderWindow& window, sf::Font& font) 
	: m_Window(window), m_Font(font) {}

void UIRenderer::clear() {
	m_Window.clear(sf::Color::White);
}

void UIRenderer::display() {
	m_Window.display();
}

void UIRenderer::renderHomeScreen() {
	drawTitle();
	drawButton(WINDOW_WIDTH/2 - BUTTON_WIDTH/2, 350, BUTTON_WIDTH, BUTTON_HEIGHT, "CONNECT", sf::Color::Black);
	drawButton(WINDOW_WIDTH/2 - BUTTON_WIDTH/2, 420, BUTTON_WIDTH, BUTTON_HEIGHT, "EXIT", sf::Color::Black);
}

void UIRenderer::renderWhiteboard(const std::vector<WhiteboardStateMachine::Line>& lines, const std::vector<WhiteboardStateMachine::Rectangle>& rectangles,
								const WhiteboardStateMachine::Line& currentLine, const WhiteboardStateMachine::Rectangle& currentRect,
								bool isDrawing, bool isRectStarted, WhiteboardStateMachine::DrawTool currentTool) {
	renderMenuBar(currentTool);
	renderDrawings(lines, rectangles, currentLine, currentRect, isDrawing, isRectStarted, currentTool);
}

void UIRenderer::renderMenuBar(WhiteboardStateMachine::DrawTool currentTool) {
	sf::RectangleShape menuBar(sf::Vector2f(WINDOW_WIDTH, MENU_BAR_HEIGHT));
	menuBar.setFillColor(sf::Color::White);
	menuBar.setOutlineColor(sf::Color::Black);
	menuBar.setOutlineThickness(2);
	m_Window.draw(menuBar);

	// temporary values for visualizing
	int buttonX = 10;
	int buttonY = 10;
	int spacing = BUTTON_WIDTH + 10;

	// come up with a better way of distinguishing which tool is currently selected
	sf::Color homeColor = sf::Color::Black;
	sf::Color markerColor = (currentTool == WhiteboardStateMachine::DrawTool::MARKER) ? sf::Color(100, 100, 100) : sf::Color::Black;
	sf::Color rectColor = (currentTool == WhiteboardStateMachine::DrawTool::RECTANGLE) ? sf::Color(100, 100, 100) : sf::Color::Black;
	sf::Color eraserColor = (currentTool == WhiteboardStateMachine::DrawTool::ERASER) ? sf::Color(100, 100, 100) : sf::Color::Black;

	drawButton(buttonX, buttonY, BUTTON_WIDTH, BUTTON_HEIGHT, "HOME", homeColor);
	buttonX += spacing;
	drawButton(buttonX, buttonY, BUTTON_WIDTH, BUTTON_HEIGHT, "MARKER", markerColor);
	buttonX += spacing;
	drawButton(buttonX, buttonY, BUTTON_WIDTH, BUTTON_HEIGHT, "RECTANGLE", rectColor);
	buttonX += spacing;
	drawButton(buttonX, buttonY, BUTTON_WIDTH, BUTTON_HEIGHT, "ERASER", eraserColor);
}

void UIRenderer::renderDrawings(const std::vector<WhiteboardStateMachine::Line>& lines, const std::vector<WhiteboardStateMachine::Rectangle>& rectangles,
							  const WhiteboardStateMachine::Line& currentLine, const WhiteboardStateMachine::Rectangle& currentRect,
							  bool isDrawing, bool isRectStarted, WhiteboardStateMachine::DrawTool currentTool) {
	// Draw completed lines
	for (const auto& line : lines) {
		if (line.points.size() < 2) continue;
		for (size_t i = 1; i < line.points.size(); ++i) {
			drawLine(line.points[i-1].position, line.points[i].position, line.points[i].color);
		}
	}

	// Draw current line being drawn
	if (isDrawing && currentTool == WhiteboardStateMachine::DrawTool::MARKER && currentLine.points.size() >= 2) {
		for (size_t i = 1; i < currentLine.points.size(); ++i) {
			drawLine(currentLine.points[i-1].position, currentLine.points[i].position, currentLine.points[i].color);
		}
	}

	// Draw completed rectangles
	for (const auto& rect : rectangles) {
		if (rect.isComplete) {
			drawRectangle(rect.start, rect.end, sf::Color::Black);
		}
	}

	// Draw current rectangle preview
	if (isRectStarted && currentTool == WhiteboardStateMachine::DrawTool::RECTANGLE) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(m_Window);
		drawRectangle(currentRect.start, sf::Vector2f(mousePos.x, mousePos.y), sf::Color(128, 128, 128));
	}
}

void UIRenderer::drawTitle() {
	sf::Text title(m_Font, "WHITEBOARD");
	title.setCharacterSize(48);
	title.setFillColor(sf::Color::Black);
	title.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - title.getGlobalBounds().size.x / 2, 200));
	m_Window.draw(title);
}

void UIRenderer::drawButton(float x, float y, float width, float height, const std::string& text, sf::Color color) {
	sf::RectangleShape button(sf::Vector2f(width, height));
	button.setPosition(sf::Vector2f(x, y));
	button.setFillColor(sf::Color::White);
	button.setOutlineColor(color);
	button.setOutlineThickness(2);
	m_Window.draw(button);

	sf::Text buttonText(m_Font, text);
	buttonText.setString(text);
	buttonText.setCharacterSize(16);
	buttonText.setFillColor(color);
	buttonText.setPosition(sf::Vector2f(x + width / 2 - buttonText.getGlobalBounds().size.x / 2,
						  y + height / 2 - buttonText.getGlobalBounds().size.y / 2));
	m_Window.draw(buttonText);
}

void UIRenderer::drawLine(sf::Vector2f start, sf::Vector2f end, sf::Color color) {
	sf::Vertex startVertex;
	startVertex.position = start;
	startVertex.color = color;

	sf::Vertex endVertex;
	endVertex.position = start;
	endVertex.color = color;
	sf::Vertex line[2] = { startVertex, endVertex };
	m_Window.draw(line, 2, sf::PrimitiveType::Lines);
}

void UIRenderer::drawRectangle(sf::Vector2f start, sf::Vector2f end, sf::Color color) {
	sf::RectangleShape rect;
	rect.setPosition(sf::Vector2f(std::min(start.x, end.x), std::min(start.y, end.y)));
	rect.setSize(sf::Vector2f(std::abs(end.x - start.x), std::abs(end.y - start.y)));
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(color);
	rect.setOutlineThickness(2);
	m_Window.draw(rect);
}
