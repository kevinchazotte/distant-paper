#pragma once

#include "IServerManager.h"

#include <mutex>
#include <string>

#include <grpcpp/server.h>

#include "DrawingService.h"

class ServerManager : public IServerManager {
public:
    ServerManager(const std::string& targetAddress);
    ~ServerManager() override;

    int Connect(const std::string& peerAddress) override;
    bool Disconnect(int connectionId) override;

    void Start() override;
    void Stop() override;
    void Update() override;

    int AssignNewConnectionId(const std::string& peerAddress) override;
    bool IsValidConnectionId(int connectionId) override;
    bool RemoveConnectionId(int connectionId) override;

    std::string ReserveObjectId() override;

private:
    int m_NumConnections = 0;
    const std::string m_TargetAddress;

    std::unique_ptr<DrawingServiceImpl> drawingService_;
    std::unique_ptr<grpc::Server> m_ServerInstance;
    std::unordered_map<int, std::string> m_OpenClients;
    std::mutex m_ServerConnectionMutex;
};
