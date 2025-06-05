#include "ServerConnectionManager.h"

#include <grpcpp/grpcpp.h>

ServerConnectionManager::ServerConnectionManager(const std::string& targetAddress) : m_TargetAddress(targetAddress) {
    m_ConnectionId = -1;
    m_ClientStreamIsOpen = false;
    m_DrawingEventListener = nullptr;
}

ServerConnectionManager::~ServerConnectionManager() {
    Disconnect();
    m_DrawingServiceStub.reset();
}

bool ServerConnectionManager::Connect() {
    std::cout << "[Client] Opening a connection to " << m_TargetAddress << std::endl;
    grpc::ChannelArguments channel_args;
    m_ServerChannel = grpc::CreateCustomChannel(m_TargetAddress, grpc::InsecureChannelCredentials(), channel_args);

    std::cout << "[Client] Testing connection with client stub..." << std::endl;
    m_ConnectionServiceStub = Whiteboard::Server::ServerConnectionService::NewStub(m_ServerChannel);

    Whiteboard::Server::ServerConnectionRequest request;
    request.set_connection(99); // hard-coded for testing

    Whiteboard::Server::ServerConnectionConfirmation reply;
    grpc::ClientContext context;
    std::chrono::system_clock::time_point deadline =
        std::chrono::system_clock::now() + std::chrono::seconds(5);
    context.set_deadline(deadline);

    std::cout << "[Client] Sending Connect RPC..." << std::endl;
    grpc::Status status = m_ConnectionServiceStub->Connect(&context, request, &reply);
    if (status.ok()) {
        m_ConnectionId = reply.connection();
        if (m_ConnectionId < 0) {
            std::cerr << "[Client] RPC failed with connection Id " << std::to_string(m_ConnectionId) << std::endl;
            return false;
        }
        m_DrawingServiceStub = Whiteboard::Drawing::DrawingService::NewStub(m_ServerChannel);
        if (!m_DrawingServiceStub) {
            std::cerr << "[Client] Failed to create DrawingService Stub." << std::endl;
            return false;
        }
    }
    else {
        std::cout << "[Client] RPC failed with error code " << status.error_code() << ": " << status.error_message() << std::endl;
        return false;
    }
    return true;
}

bool ServerConnectionManager::Disconnect() {
    if (m_ConnectionServiceStub && m_ConnectionId != -1) {
        Whiteboard::Server::ServerConnectionRequest request;
        request.set_connection(m_ConnectionId); // hard-coded for testing
        Whiteboard::Server::ServerConnectionConfirmation reply;
        grpc::ClientContext context;
        std::chrono::system_clock::time_point deadline =
            std::chrono::system_clock::now() + std::chrono::seconds(5);
        context.set_deadline(deadline);

        std::cout << "[Client] Sending Connect RPC..." << std::endl;
        grpc::Status status = m_ConnectionServiceStub->Disconnect(&context, request, &reply);
        if (!status.ok() || reply.connection() == 98) {
            std::cout << "[Client] RPC failed with error code " << status.error_code() << " and connection id " << std::to_string(reply.connection()) << std::endl;
        }
        m_ConnectionServiceStub.reset();
        m_ConnectionId = -1;
    }

    CloseSubscriberStream();
    m_ServerChannel.reset();
    return true;
}

bool ServerConnectionManager::OpenSubscriberStream(IDrawingEventListener* listener) {
    if (!m_DrawingServiceStub || m_ConnectionId < 0 || m_ClientStreamIsOpen) {
        std::cout << " returning false here ... " << std::endl;
        return false;
    }
    m_DrawingEventListener = listener;
    m_ClientStreamContext = std::make_unique<grpc::ClientContext>();

    m_ClientStream = m_DrawingServiceStub->OpenDrawingStream(m_ClientStreamContext.get());
    if (!m_ClientStream) {
        std::cerr << "[Client] Failed to open drawing stream." << std::endl;
        return false;
    }

    Whiteboard::Drawing::StreamEvent initialEvent;
    initialEvent.set_connectionid(m_ConnectionId);
    if (!m_ClientStream->Write(initialEvent)) {
        std::cerr << "[Client] Initial event message failed." << std::endl;
    }

    m_ClientStreamIsOpen = true;
    m_EventListenerThread = std::thread(&ServerConnectionManager::ReadSubscriberStream, this);
    std::cout << "[Client] Started drawing stream..." << std::endl;
    return true;
}

void ServerConnectionManager::ReadSubscriberStream() {
    Whiteboard::Drawing::StreamEvent event;
    while (m_ClientStreamIsOpen && m_ClientStream && m_ClientStream->Read(&event)) {
        if (m_DrawingEventListener) {
            if (event.has_drawingevent() && event.drawingevent().has_drawable()) {
                m_DrawingEventListener->OnDrawableAdded(event.drawingevent().drawable());
            }
            else if (event.has_eraseevent() && event.eraseevent().has_drawable()) {
                m_DrawingEventListener->OnDrawableErased(event.eraseevent().drawable());
            }
        }
    }
    if (m_ClientStreamIsOpen == true) { // if client stream was unintentionally closed
        grpc::Status status = m_ClientStream->Finish();
        if (m_DrawingEventListener) {
            m_DrawingEventListener->OnListenerDisconnected(status);
        }
    }
}

void ServerConnectionManager::CloseSubscriberStream() {
    if (!m_ClientStreamIsOpen.exchange(false)) { // send close signal, do nothing if client stream is already closed
        return;
    }

    if (m_ClientStreamContext) {
        m_ClientStreamContext->TryCancel();
    }

    if (m_EventListenerThread.joinable()) {
        m_EventListenerThread.join();
    }

    if (m_ClientStream) {
        m_ClientStream->WritesDone();
        m_ClientStream->Finish();
    }

    m_ClientStream.reset();
    m_ClientStreamContext.reset();
    m_DrawingEventListener = nullptr;
}

bool ServerConnectionManager::SendDrawable(const Whiteboard::Types::Drawable& drawable) {
    if (!m_ClientStream || !m_ClientStreamIsOpen) {
        std::cerr << "[Client] Failed to send drawable." << std::endl;
        return false;
    }
    Whiteboard::Drawing::StreamEvent event;
    event.set_connectionid(m_ConnectionId);
    *event.mutable_drawingevent()->mutable_drawable() = drawable;
    return m_ClientStream->Write(event);
}

bool ServerConnectionManager::SendErase(const Whiteboard::Types::Drawable& drawable) {
    if (!m_ClientStream || !m_ClientStreamIsOpen) {
        std::cerr << "[Client] Failed to send drawable." << std::endl;
        return false;
    }
    Whiteboard::Drawing::StreamEvent event;
    event.set_connectionid(m_ConnectionId);
    *event.mutable_eraseevent()->mutable_drawable() = drawable;
    return m_ClientStream->Write(event);
}
