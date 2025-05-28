#pragma once

#include "IDrawingManager.h"

#include <memory>

#include "IServerManager.h"

class DrawingManager : public IDrawingManager {
public:
    DrawingManager(std::shared_ptr<IServerManager> serverManager);
    virtual ~DrawingManager() = default;

    void startDrawing(sf::Vector2f position, WhiteboardStateMachine::DrawTool tool) override;
    void updateDrawing(sf::Vector2f position, WhiteboardStateMachine::DrawTool tool) override;
    void endDrawing(WhiteboardStateMachine::DrawTool tool) override;
    void eraseAt(sf::Vector2f position) override;
    void clear() override;

    std::vector<WhiteboardStateMachine::Line>& getLines() override { return m_Lines; }
    std::vector<WhiteboardStateMachine::Rectangle>& getRectangles() override { return m_Rectangles; }

    WhiteboardStateMachine::Line& getCurrentLine() override { return m_CurrentLine; }
    WhiteboardStateMachine::Rectangle& getCurrentRect() override { return m_CurrentRect; }

    bool isDrawing() override { return m_IsDrawing; }
    bool isRectStarted() override { return m_IsRectStarted; }

private:
    std::shared_ptr<IServerManager> m_ServerManager;

    std::vector<WhiteboardStateMachine::Line> m_Lines;
    std::vector<WhiteboardStateMachine::Rectangle> m_Rectangles;

    WhiteboardStateMachine::Line m_CurrentLine;
    WhiteboardStateMachine::Rectangle m_CurrentRect;

    bool m_IsDrawing;
    bool m_IsRectStarted;

    static const int kEraserSize = 20;
};