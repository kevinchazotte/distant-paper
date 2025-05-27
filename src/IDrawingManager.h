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

    const std::vector<WhiteboardStateMachine::Line>& getLines() const { return m_Lines; }
    const std::vector<WhiteboardStateMachine::Rectangle>& getRectangles() const { return m_Rectangles; }

    const WhiteboardStateMachine::Line& getCurrentLine() const { return m_CurrentLine; }
    const WhiteboardStateMachine::Rectangle& getCurrentRect() const { return m_CurrentRect; }

    bool isDrawing() const { return m_IsDrawing; }
    bool isRectStarted() const { return m_IsRectStarted; }

private:
    std::vector<WhiteboardStateMachine::Line> m_Lines;
    std::vector<WhiteboardStateMachine::Rectangle> m_Rectangles;

    WhiteboardStateMachine::Line m_CurrentLine;
    WhiteboardStateMachine::Rectangle m_CurrentRect;

    bool m_IsDrawing;
    bool m_IsRectStarted;
};