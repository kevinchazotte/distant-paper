#pragma once

#include <string>

#include <SFML/Graphics.hpp>

#include "drawing_types.grpc.pb.h"
#include "drawing_types.pb.h"

#include "WhiteboardStateMachine.h"

class IRenderer {
public:
	virtual ~IRenderer() = default;
	virtual void Clear() = 0;
	virtual void Display() = 0;
	virtual void RenderHomeScreen() = 0;
	virtual void RenderWhiteboard(const std::vector<Whiteboard::Line>& lines, const std::vector<Whiteboard::Rectangle>& rectangles,
								const Whiteboard::Line& currentLine, const Whiteboard::Rectangle& currentRectangle, const sf::CircleShape& cursorCircle,
								bool isDrawing, WhiteboardStateMachine::DrawTool currentTool) = 0;
};
