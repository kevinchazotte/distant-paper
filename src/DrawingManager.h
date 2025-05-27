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

private:
    std::shared_ptr<IServerManager> m_ServerManager;

    bool m_IsDrawing;
    bool m_IsRectStarted;

    static const int kEraserSize = 20;
};