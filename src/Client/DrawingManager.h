#pragma once

#include "IDrawingEventListener.h"
#include "IDrawingManager.h"

#include <memory>
#include <mutex>
#include <unordered_map>

#include "drawing_service.grpc.pb.h"
#include "drawing_service.pb.h"

#include "ServerConnectionManager.h"

class DrawingManager : public IDrawingManager, public IDrawingEventListener {
public:
    DrawingManager(std::shared_ptr<ServerConnectionManager> serverConnectionManager);
    ~DrawingManager();

    void OnDrawableAdded(const Whiteboard::Types::Drawable& drawable) override;
    void OnDrawableErased(const Whiteboard::Types::Drawable& drawable) override;
    void OnListenerDisconnected(const grpc::Status& status) override;

    void StartDrawing(sf::Vector2f position, WhiteboardStateMachine::DrawTool tool) override;
    void UpdateDrawing(sf::Vector2f position, WhiteboardStateMachine::DrawTool tool) override;
    void EndDrawingAndUpdate(WhiteboardStateMachine::DrawTool tool) override;
    void StopDrawing(WhiteboardStateMachine::DrawTool tool) override;
    void EraseAt(sf::Vector2f position) override;
    void Clear() override;

    std::unordered_map<std::string, Whiteboard::Types::Drawable> GetDrawables() override;

    Whiteboard::Types::Line GetCurrentLine() override { return m_CurrentLine; }
    Whiteboard::Types::Rectangle GetCurrentRectangle() override { return m_CurrentRectangle; }

    sf::CircleShape GetCursorCircle() override { return m_CursorCircle; }
    bool IsDrawing() override { return m_IsDrawing; }

private:
    std::unordered_map<std::string, Whiteboard::Types::Drawable> m_Drawables;
    std::mutex m_DrawablesMutex;

    Whiteboard::Types::Line m_CurrentLine;
    Whiteboard::Types::Rectangle m_CurrentRectangle;

    sf::CircleShape m_CursorCircle;
    bool m_IsDrawing;

    static const int kEraserSize = 20;

    std::shared_ptr<ServerConnectionManager> m_ServerConnectionManager;
};
