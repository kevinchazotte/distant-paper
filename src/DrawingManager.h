#pragma once

#include "IDrawingManager.h"

#include <memory>

#include "IServerManager.h"

class DrawingManager : public IDrawingManager {
public:
    DrawingManager(std::shared_ptr<IServerManager> serverManager);
    ~DrawingManager();

    void StartDrawing(sf::Vector2f position, WhiteboardStateMachine::DrawTool tool) override;
    void UpdateDrawing(sf::Vector2f position, WhiteboardStateMachine::DrawTool tool) override;
    void EndDrawingAndUpdate(WhiteboardStateMachine::DrawTool tool) override;
    void StopDrawing(WhiteboardStateMachine::DrawTool tool) override;
    void EraseAt(sf::Vector2f position) override;
    void Clear() override;

    std::vector<Whiteboard::Line> GetLines() override { return m_Lines; }
    std::vector<Whiteboard::Rectangle> GetRectangles() override { return m_Rectangles; }

    Whiteboard::Line GetCurrentLine() override { return m_CurrentLine; }
    Whiteboard::Rectangle GetCurrentRectangle() override { return m_CurrentRectangle; }

    sf::CircleShape GetCursorCircle() override { return m_CursorCircle; }
    bool IsDrawing() override { return m_IsDrawing; }

private:
    std::shared_ptr<IServerManager> m_ServerManager;

    std::vector<Whiteboard::Line> m_Lines;
    std::vector<Whiteboard::Rectangle> m_Rectangles;

    Whiteboard::Line m_CurrentLine;
    Whiteboard::Rectangle m_CurrentRectangle;

    sf::CircleShape m_CursorCircle;
    bool m_IsDrawing;

    static const int kEraserSize = 20;
};
