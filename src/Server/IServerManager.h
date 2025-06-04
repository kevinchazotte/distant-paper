#pragma once

#include <string>

class IServerManager {
public:
    virtual ~IServerManager() = default;

    virtual int Connect(const std::string& peerAddress) = 0;
    virtual bool Disconnect(int connectionId) = 0;

    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void Update() = 0;

    virtual int AssignNewConnectionId(const std::string& peerAddress) = 0;
    virtual bool IsValidConnectionId(int connectionId) = 0;
    virtual bool RemoveConnectionId(int connectionId) = 0;

    virtual std::string ReserveObjectId() = 0;
};
