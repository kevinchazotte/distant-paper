#include "ServerManager.h"

#include <grpcpp/grpcpp.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include "greeter.pb.h"

// Server implementation
class GreeterServiceImpl final : public helloworld::Greeter::Service {
public:
    explicit GreeterServiceImpl(ServerManager* manager) : serverManager_(manager) {}

    grpc::Status SayHello(grpc::ServerContext* context,
        const helloworld::HelloRequest* request,
        helloworld::HelloReply* reply) override {
        int newConnectionId = serverManager_->AssignNewConnectionId(context->peer());
        reply->set_connection(newConnectionId);
        std::cout << "[Server Thread] Responded to request with input connection: " << request->connection() << std::endl;
        return grpc::Status::OK;
    }
private:
    ServerManager* serverManager_;
};

ServerManager::ServerManager(const std::string targetAddress) : m_TargetAddress(targetAddress) {
    
}

std::pair<grpc::Status, helloworld::HelloReply> ServerManager::Connect() {
    std::cout << "[Client] Opening a connection with id " << std::to_string(m_NumConnections) << " to " << m_TargetAddress << std::endl;

    // client side code - will need to migrate out of this class soon
    grpc::ChannelArguments channel_args;
    std::shared_ptr<grpc::Channel> channel = grpc::CreateCustomChannel(m_TargetAddress, grpc::InsecureChannelCredentials(), channel_args);

    std::cout << "[Client] Testing connection with client stub..." << std::endl;
    std::unique_ptr<helloworld::Greeter::Stub> stub = helloworld::Greeter::NewStub(channel);

    helloworld::HelloRequest request;
    request.set_connection(99); // hard-coded for testing

    helloworld::HelloReply reply;
    grpc::ClientContext context;

    std::chrono::system_clock::time_point deadline =
        std::chrono::system_clock::now() + std::chrono::seconds(5);
    context.set_deadline(deadline);

    std::cout << "[Client] Sending SayHello RPC..." << std::endl;
    grpc::Status status = stub->SayHello(&context, request, &reply);
    return std::make_pair(status, reply);
}

bool ServerManager::Disconnect(int connectionId) {
    return RemoveConnectionId(connectionId);
}

void ServerManager::SendLine(const Whiteboard::Line& line) {

}

void ServerManager::SendRectangle(const Whiteboard::Rectangle& rect) {

}

void ServerManager::SendErase(sf::Vector2f position) {

}

void ServerManager::Start() {
    GreeterServiceImpl service_impl(this);
    grpc::ServerBuilder builder;
    grpc::ServerContext context;
    // Listen on the given address using insecure credentials
    builder.AddListeningPort(m_TargetAddress, grpc::InsecureServerCredentials());
    builder.RegisterService(&service_impl);

    // Assemble and start the server
    m_ServerInstance = builder.BuildAndStart();
    if (m_ServerInstance) {
        std::cout << "[Server Thread] Server listening on " << m_TargetAddress << std::endl;
        // Keep server running until Shutdown() is called from another thread
        m_ServerInstance->Wait();
        std::cout << "[Server Thread] Server shutdown complete." << std::endl;
    }
    else {
        std::cerr << "[Server Thread] Failed to start server on " << m_TargetAddress << std::endl;
    }
}

void ServerManager::Stop() {
    m_ServerInstance->Shutdown(std::chrono::system_clock::now() + std::chrono::seconds(1));
}

void ServerManager::Update() {

}

int ServerManager::AssignNewConnectionId(std::string peerAddress) {
    std::lock_guard<std::mutex> lock(m_ServerMutex);
    m_OpenClients.insert(std::make_pair(m_NumConnections, peerAddress));
    return m_NumConnections++;
}

bool ServerManager::RemoveConnectionId(int connectionId) {
    std::lock_guard<std::mutex> lock(m_ServerMutex);
    if (m_OpenClients.find(connectionId) != m_OpenClients.end()) {
        m_OpenClients.erase(connectionId);
        m_NumConnections--;
        return true;
    }
    return false;
}
