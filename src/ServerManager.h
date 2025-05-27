#pragma once

#include "IServerManager.h"

#include "WhiteboardStateMachine.h"

class ServerManager : public IServerManager {
public:
    ServerManager();
    virtual ~ServerManager() = default;

    void connect() override;
    void disconnect() override;
    void sendLine(const WhiteboardStateMachine::Line& line) override;
    void sendRectangle(const WhiteboardStateMachine::Rectangle& rect) override;
    void sendErase(sf::Vector2f position) override;
    void update() override;
};
