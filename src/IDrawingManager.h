#pragma once

#include <SFML/Graphics.hpp>

#include "WhiteboardStateMachine.h"

class IDrawingManager {
public:
    virtual ~IDrawingManager() = default;

    virtual void StartDrawing(sf::Vector2f position, WhiteboardStateMachine::DrawTool tool) = 0;
    virtual void UpdateDrawing(sf::Vector2f position, WhiteboardStateMachine::DrawTool tool) = 0;
    virtual void EndDrawing(WhiteboardStateMachine::DrawTool tool) = 0;
    virtual void EraseAt(sf::Vector2f position) = 0;
    virtual void Clear() = 0;

    virtual std::vector<WhiteboardStateMachine::Line>& GetLines() = 0;
    virtual std::vector<WhiteboardStateMachine::Rectangle>& GetRectangles() = 0;

    virtual WhiteboardStateMachine::Line& GetCurrentLine() = 0;
    virtual WhiteboardStateMachine::Rectangle& GetCurrentRectangle() = 0;

    virtual bool IsDrawing() = 0;
    virtual bool IsRectangleStarted() = 0;
};