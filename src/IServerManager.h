#pragma once

#include <grpcpp/server.h>

#include "drawing_types.grpc.pb.h"
#include "drawing_types.pb.h"
#include "greeter.grpc.pb.h"

#include "WhiteboardStateMachine.h"

class IServerManager {
public:
    virtual ~IServerManager() = default;

    virtual std::pair<grpc::Status, helloworld::HelloReply> Connect() = 0;
    virtual bool Disconnect(int connectionId) = 0;

    virtual void SendLine(const Whiteboard::Line& line) = 0;
    virtual void SendRectangle(const Whiteboard::Rectangle& rect) = 0;
    virtual void SendErase(sf::Vector2f position) = 0;

    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void Update() = 0;

    virtual int AssignNewConnectionId(std::string peerAddress) = 0;
    virtual bool RemoveConnectionId(int connectionId) = 0;
};
