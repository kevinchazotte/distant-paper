#pragma once

#include "WhiteboardStateMachine.h"

class IServerManager {
public:
    virtual ~IServerManager() = default;
    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual void sendLine(const WhiteboardStateMachine::Line& line) = 0;
    virtual void sendRectangle(const WhiteboardStateMachine::Rectangle& rect) = 0;
    virtual void sendErase(sf::Vector2f position) = 0;
    virtual void update() = 0;
private:
    int m_NumConnections = 0;
};
