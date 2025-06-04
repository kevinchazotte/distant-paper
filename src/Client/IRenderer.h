#pragma once

#include <string>
#include <unordered_map>

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
	virtual void RenderWhiteboard(const std::unordered_map<std::string, Whiteboard::Types::Drawable>& drawables,
		const Whiteboard::Types::Line& currentLine, const Whiteboard::Types::Rectangle& currentRectangle, const sf::CircleShape& cursorCircle,
		bool isDrawing, WhiteboardStateMachine::DrawTool currentTool) = 0;
};
