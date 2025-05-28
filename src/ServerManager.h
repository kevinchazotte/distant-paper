#pragma once

#include "IServerManager.h"

#include <grpcpp/server.h>

#include "WhiteboardStateMachine.h"

class ServerManager : public IServerManager {
public:
    ServerManager(const std::string targetAddress);
    virtual ~ServerManager() = default;

    void Connect() override;
    void Disconnect() override;
    void SendLine(const WhiteboardStateMachine::Line& line) override;
    void SendRectangle(const WhiteboardStateMachine::Rectangle& rect) override;
    void SendErase(sf::Vector2f position) override;
    void Start() override;
    void Stop() override;
    void Update() override;
private:
    int m_NumConnections = 0;
    const std::string m_TargetAddress;
    std::unique_ptr<grpc::Server> m_ServerInstance;
    std::unordered_map<int, std::shared_ptr<grpc::Channel>> m_OpenClients;
};
