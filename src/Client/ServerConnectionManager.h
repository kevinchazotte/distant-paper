#pragma once

#include "IServerConnectionManager.h"

#include <mutex>
#include <string>

#include "WhiteboardStateMachine.h"

class ServerConnectionManager : public IServerConnectionManager {
public:
    ServerConnectionManager(const std::string& targetAddress);
    virtual ~ServerConnectionManager() = default;

    bool Connect() override;
    bool Disconnect() override;
    void Update() override;

    void SendLine(const Whiteboard::Line& line) override;
    void SendRectangle(const Whiteboard::Rectangle& rect) override;
    void SendErase(sf::Vector2f position) override;

    int GetConnectionId() override { return m_ConnectionId; }
private:
    int m_ConnectionId;
    const std::string m_TargetAddress;

};
