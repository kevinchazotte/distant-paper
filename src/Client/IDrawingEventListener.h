#pragma once

#include "drawing_types.grpc.pb.h"
#include "drawing_types.pb.h"

class IDrawingEventListener {
public:
    virtual ~IDrawingEventListener() = default;

    virtual void OnDrawableAdded(const Whiteboard::Types::Drawable& drawable) = 0;
    virtual void OnDrawableErased(const Whiteboard::Types::Drawable& drawable) = 0;
    virtual void OnListenerDisconnected(const grpc::Status& status) = 0;
};
