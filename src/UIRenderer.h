#pragma once

#include "IRenderer.h"

class UIRenderer : public IRenderer {
public:
	UIRenderer(sf::RenderWindow& window, sf::Font font);
	virtual ~UIRenderer() = default;

	void Clear() override;
	void Display() override;
	void RenderHomeScreen() override;
	void RenderWhiteboard(const std::vector<Whiteboard::Line>& lines, const std::vector<Whiteboard::Rectangle>& rectangles,
						const Whiteboard::Line& currentLine, const Whiteboard::Rectangle& currentRectangle, const sf::CircleShape& cursorCircle,
						bool isDrawing, WhiteboardStateMachine::DrawTool currentTool) override;

private:
	sf::RenderWindow& m_RenderWindow;
	sf::Font m_Font;

	static const int kMenuBarHeight = 60;
	static const int kButtonWidth = 120;
	static const int kButtonHeight = 40;

	void DrawTitle();
	void DrawButton(float x, float y, float width, float height, const std::string& text, sf::Color color);
	void DrawLine(const std::array<sf::Vertex, 2>& vertexArray);
	void DrawRectangle(const sf::RectangleShape& rectangleShape);
	void DrawCircle(const sf::CircleShape& circleShape);
	void RenderDrawings(const std::vector<Whiteboard::Line>& lines, const std::vector<Whiteboard::Rectangle>& rectangles,
					  const Whiteboard::Line& currentLine, const Whiteboard::Rectangle& currentRectangle,
					  bool isDrawing, WhiteboardStateMachine::DrawTool currentTool);
	void RenderCursor(const sf::CircleShape& cursorCircle, bool isDrawing, WhiteboardStateMachine::DrawTool currentTool);
	void RenderMenuBar(WhiteboardStateMachine::DrawTool currentTool);
};
