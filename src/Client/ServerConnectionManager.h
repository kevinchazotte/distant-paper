#pragma once

#include <atomic>
#include <mutex>
#include <string>
#include <thread>

#include <grpcpp/client_context.h>

#include "drawing_service.grpc.pb.h"
#include "drawing_service.pb.h"
#include "drawing_types.grpc.pb.h"
#include "drawing_types.pb.h"
#include "server_connection.grpc.pb.h"
#include "server_connection.pb.h"

#include "IDrawingEventListener.h"
#include "WhiteboardStateMachine.h"

class ServerConnectionManager {
public:
    ServerConnectionManager(const std::string& targetAddress);
    ~ServerConnectionManager();

    bool Connect();
    bool Disconnect();
    bool OpenSubscriberStream(IDrawingEventListener* listener);
    void ReadSubscriberStream();
    void CloseSubscriberStream();

    bool SendDrawable(const Whiteboard::Types::Drawable& drawable);
    bool SendErase(const Whiteboard::Types::Drawable& drawable);

    int GetConnectionId() { return m_ConnectionId; }

private:
    int m_ConnectionId;
    const std::string m_TargetAddress;

    std::shared_ptr<grpc::Channel> m_ServerChannel;
    std::unique_ptr<Whiteboard::Drawing::DrawingService::Stub> m_DrawingServiceStub;
    std::unique_ptr<Whiteboard::Server::ServerConnectionService::Stub> m_ConnectionServiceStub;

    IDrawingEventListener* m_DrawingEventListener;
    std::unique_ptr<grpc::ClientReaderWriter<Whiteboard::Drawing::StreamEvent, Whiteboard::Drawing::StreamEvent>> m_ClientStream;
    std::unique_ptr<grpc::ClientContext> m_ClientStreamContext;
    std::thread m_EventListenerThread;
    std::atomic<bool> m_ClientStreamIsOpen;
};
