#pragma once

#include "IRenderer.h"

class UIRenderer : public IRenderer {
public:
	UIRenderer(sf::RenderWindow& window, sf::Font& font);
	virtual ~UIRenderer() = default;

	void clear() override;
	void display() override;
	void renderHomeScreen() override;
	void renderWhiteboard(const std::vector<WhiteboardStateMachine::Line>& lines, const std::vector<WhiteboardStateMachine::Rectangle>& rectangles,
						const WhiteboardStateMachine::Line& currentLine, const WhiteboardStateMachine::Rectangle& currentRect,
						bool isDrawing, bool isRectStarted, WhiteboardStateMachine::DrawTool currentTool) override;

private:
	sf::RenderWindow& m_Window;
	sf::Font& m_Font;

	static const int kMenuBarHeight = 60;
	static const int kButtonWidth = 120;
	static const int kButtonHeight = 40;

	void drawTitle();
	void drawButton(float x, float y, float width, float height, const std::string& text, sf::Color color);
	void drawLine(sf::Vector2f start, sf::Vector2f end, sf::Color color);
	void drawRectangle(sf::Vector2f start, sf::Vector2f end, sf::Color color);
	void renderDrawings(const std::vector<WhiteboardStateMachine::Line>& lines, const std::vector<WhiteboardStateMachine::Rectangle>& rectangles,
					  const WhiteboardStateMachine::Line& currentLine, const WhiteboardStateMachine::Rectangle& currentRect,
					  bool isDrawing, bool isRectStarted, WhiteboardStateMachine::DrawTool currentTool);
	void renderMenuBar(WhiteboardStateMachine::DrawTool currentTool);
};
