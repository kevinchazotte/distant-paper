#pragma once

#include <string>

#include <SFML/Graphics.hpp>

#include "WhiteboardStateMachine.h"

class IRenderer {
public:
	virtual ~IRenderer() = default;
	virtual void Clear() = 0;
	virtual void Display() = 0;
	virtual void RenderHomeScreen() = 0;
	virtual void RenderWhiteboard(const std::vector<WhiteboardStateMachine::Line>& lines, const std::vector<WhiteboardStateMachine::Rectangle>& rectangles,
								const WhiteboardStateMachine::Line& currentLine, const WhiteboardStateMachine::Rectangle& currentRect,
								bool isDrawing, bool isRectStarted, WhiteboardStateMachine::DrawTool currentTool) = 0;
};
