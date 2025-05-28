#pragma once

#include "IRenderer.h"

class UIRenderer : public IRenderer {
public:
	UIRenderer(sf::RenderWindow& window, sf::Font font);
	virtual ~UIRenderer() = default;

	void Clear() override;
	void Display() override;
	void RenderHomeScreen() override;
	void RenderWhiteboard(const std::vector<WhiteboardStateMachine::Line>& lines, const std::vector<WhiteboardStateMachine::Rectangle>& rectangles,
						const WhiteboardStateMachine::Line& currentLine, const WhiteboardStateMachine::Rectangle& currentRect,
						bool isDrawing, bool isRectStarted, WhiteboardStateMachine::DrawTool currentTool) override;

private:
	sf::RenderWindow& m_RenderWindow;
	sf::Font m_Font;

	static const int kMenuBarHeight = 60;
	static const int kButtonWidth = 120;
	static const int kButtonHeight = 40;

	void DrawTitle();
	void DrawButton(float x, float y, float width, float height, const std::string& text, sf::Color color);
	void DrawLine(sf::Vector2f start, sf::Vector2f end, sf::Color color);
	void DrawRectangle(sf::Vector2f start, sf::Vector2f end, sf::Color color);
	void RenderDrawings(const std::vector<WhiteboardStateMachine::Line>& lines, const std::vector<WhiteboardStateMachine::Rectangle>& rectangles,
					  const WhiteboardStateMachine::Line& currentLine, const WhiteboardStateMachine::Rectangle& currentRect,
					  bool isDrawing, bool isRectStarted, WhiteboardStateMachine::DrawTool currentTool);
	void RenderMenuBar(WhiteboardStateMachine::DrawTool currentTool);
};
