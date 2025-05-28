#pragma once

#include "WhiteboardStateMachine.h"

class IServerManager {
public:
    virtual ~IServerManager() = default;
    virtual void Connect() = 0;
    virtual void Disconnect() = 0;
    virtual void SendLine(const WhiteboardStateMachine::Line& line) = 0;
    virtual void SendRectangle(const WhiteboardStateMachine::Rectangle& rect) = 0;
    virtual void SendErase(sf::Vector2f position) = 0;
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void Update() = 0;
};
