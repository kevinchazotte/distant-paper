#pragma once

#include <grpcpp/server.h>

#include "drawing_types.grpc.pb.h"
#include "drawing_types.pb.h"

#include "WhiteboardStateMachine.h"

class IServerConnectionManager {
public:
    virtual ~IServerConnectionManager() = default;

    virtual bool Connect() = 0;
    virtual bool Disconnect() = 0;
    virtual void Update() = 0;

    virtual void SendLine(const Whiteboard::Line& line) = 0;
    virtual void SendRectangle(const Whiteboard::Rectangle& rect) = 0;
    virtual void SendErase(sf::Vector2f position) = 0;

    virtual int GetConnectionId() = 0;
};
