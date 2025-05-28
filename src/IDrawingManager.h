#pragma once

#include <SFML/Graphics.hpp>

#include "WhiteboardStateMachine.h"

class IDrawingManager {
public:
    virtual ~IDrawingManager() = default;

    virtual void startDrawing(sf::Vector2f position, WhiteboardStateMachine::DrawTool tool) = 0;
    virtual void updateDrawing(sf::Vector2f position, WhiteboardStateMachine::DrawTool tool) = 0;
    virtual void endDrawing(WhiteboardStateMachine::DrawTool tool) = 0;
    virtual void eraseAt(sf::Vector2f position) = 0;
    virtual void clear() = 0;

    virtual std::vector<WhiteboardStateMachine::Line>& getLines() = 0;
    virtual std::vector<WhiteboardStateMachine::Rectangle>& getRectangles() = 0;

    virtual WhiteboardStateMachine::Line& getCurrentLine() = 0;
    virtual WhiteboardStateMachine::Rectangle& getCurrentRect() = 0;

    virtual bool isDrawing() = 0;
    virtual bool isRectStarted() = 0;
};