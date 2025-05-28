#pragma once

#include "IDrawingManager.h"

#include <memory>

#include "IServerManager.h"

class DrawingManager : public IDrawingManager {
public:
    DrawingManager(std::shared_ptr<IServerManager> serverManager);
    virtual ~DrawingManager() = default;

    void StartDrawing(sf::Vector2f position, WhiteboardStateMachine::DrawTool tool) override;
    void UpdateDrawing(sf::Vector2f position, WhiteboardStateMachine::DrawTool tool) override;
    void EndDrawing(WhiteboardStateMachine::DrawTool tool) override;
    void EraseAt(sf::Vector2f position) override;
    void Clear() override;

    std::vector<WhiteboardStateMachine::Line>& GetLines() override { return m_Lines; }
    std::vector<WhiteboardStateMachine::Rectangle>& GetRectangles() override { return m_Rectangles; }

    WhiteboardStateMachine::Line& GetCurrentLine() override { return m_CurrentLine; }
    WhiteboardStateMachine::Rectangle& GetCurrentRectangle() override { return m_CurrentRectangle; }

    bool IsDrawing() override { return m_IsDrawing; }
    bool IsRectangleStarted() override { return m_IsRectStarted; }

private:
    std::shared_ptr<IServerManager> m_ServerManager;

    std::vector<WhiteboardStateMachine::Line> m_Lines;
    std::vector<WhiteboardStateMachine::Rectangle> m_Rectangles;

    WhiteboardStateMachine::Line m_CurrentLine;
    WhiteboardStateMachine::Rectangle m_CurrentRectangle;

    bool m_IsDrawing;
    bool m_IsRectStarted;

    static const int kEraserSize = 20;
};