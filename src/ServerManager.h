#pragma once

#include "IServerManager.h"

#include <mutex>
#include <string>

#include "WhiteboardStateMachine.h"

class ServerManager : public IServerManager {
public:
    ServerManager(const std::string targetAddress);
    virtual ~ServerManager() = default;

    std::pair<grpc::Status, helloworld::HelloReply> Connect() override;
    bool Disconnect(int connectionId) override;

    void SendLine(const Whiteboard::Line& line) override;
    void SendRectangle(const Whiteboard::Rectangle& rect) override;
    void SendErase(sf::Vector2f position) override;

    void Start() override;
    void Stop() override;
    void Update() override;

    int AssignNewConnectionId(std::string peerAddress) override;
    bool RemoveConnectionId(int connectionId) override;
private:
    int m_NumConnections = 0;
    const std::string m_TargetAddress;

    std::unique_ptr<grpc::Server> m_ServerInstance;
    std::unordered_map<int, std::string> m_OpenClients;
    std::mutex m_ServerMutex;
};
