#pragma once

#include "SFML/Graphics.hpp"

#include "drawing_types.grpc.pb.h"
#include "drawing_types.pb.h"

class IServerManager {
public:
    virtual ~IServerManager() = default;

    virtual bool Disconnect(int connectionId) = 0;

    virtual void BroadcastDrawable(const Whiteboard::Types::Drawable& drawable) = 0;
    virtual void BroadcastErase(sf::Vector2f position) = 0;

    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void Update() = 0;

    virtual int AssignNewConnectionId(std::string peerAddress) = 0;
    virtual bool RemoveConnectionId(int connectionId) = 0;

    virtual std::string ReserveObjectId() = 0;
};
