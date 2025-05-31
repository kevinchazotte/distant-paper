#pragma once

#include <SFML/Graphics.hpp>

#include "drawing_types.grpc.pb.h"
#include "drawing_types.pb.h"

#include "WhiteboardStateMachine.h"

class IDrawingManager {
public:
    virtual ~IDrawingManager() = default;

    virtual void StartDrawing(sf::Vector2f position, WhiteboardStateMachine::DrawTool tool) = 0;
    virtual void UpdateDrawing(sf::Vector2f position, WhiteboardStateMachine::DrawTool tool) = 0;
    virtual void EndDrawingAndUpdate(WhiteboardStateMachine::DrawTool tool) = 0;
    virtual void StopDrawing(WhiteboardStateMachine::DrawTool tool) = 0;
    virtual void EraseAt(sf::Vector2f position) = 0;
    virtual void Clear() = 0;

    virtual std::vector<Whiteboard::Line> GetLines() = 0;
    virtual std::vector<Whiteboard::Rectangle> GetRectangles() = 0;

    virtual Whiteboard::Line GetCurrentLine() = 0;
    virtual Whiteboard::Rectangle GetCurrentRectangle() = 0;

    virtual sf::CircleShape GetCursorCircle() = 0;
    virtual bool IsDrawing() = 0;
};
