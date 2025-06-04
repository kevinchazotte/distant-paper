#include "ServerManager.h"

#include <random>
#include <sstream>

#include <grpcpp/grpcpp.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include "server_connection.grpc.pb.h"
#include "server_connection.pb.h"

// Server implementation
class ServerConnectionServiceImpl final : public Whiteboard::Server::ServerConnectionService::Service {
public:
    explicit ServerConnectionServiceImpl(ServerManager* manager) : serverManager_(manager) {}

    grpc::Status Connect(grpc::ServerContext* context,
        const Whiteboard::Server::ServerConnectionRequest* request,
        Whiteboard::Server::ServerConnectionConfirmation* reply) override {
        int newConnectionId = serverManager_->Connect(context->peer());
        reply->set_connection(newConnectionId);
        std::cout << "[Server Thread] Responded to request with input connection: " << request->connection() << std::endl;
        return grpc::Status::OK;
    }

    grpc::Status Disconnect(grpc::ServerContext* context,
        const Whiteboard::Server::ServerConnectionRequest* request,
        Whiteboard::Server::ServerConnectionConfirmation* reply) override {
        bool disconnected = serverManager_->Disconnect(request->connection());
        if (disconnected) {
            reply->set_connection(99); // hard-coded for testing
        }
        else {
            reply->set_connection(98); // hard-coded for testing
        }
        std::cout << "[Server Thread] Responded to request with input connection: " << request->connection() << std::endl;
        return grpc::Status::OK;
    }
private:
    ServerManager* serverManager_;
};

ServerManager::ServerManager(const std::string& targetAddress) : m_TargetAddress(targetAddress) {
    drawingService_ = std::make_unique<DrawingServiceImpl>(this);
}

ServerManager::~ServerManager() {
    drawingService_.reset();
}

int ServerManager::Connect(const std::string& peerAddress) {
    std::lock_guard<std::mutex> lock(m_ServerConnectionMutex);
    int connectionId = AssignNewConnectionId(peerAddress);
    if (connectionId >= 0) {
        m_OpenClients.insert(std::make_pair(connectionId, peerAddress));
        return connectionId;
    }
    return -1;
}

bool ServerManager::Disconnect(int connectionId) {
    bool closedDrawingStream = false;
    if (drawingService_) {
        closedDrawingStream = drawingService_->CloseDrawingStream(connectionId);
    }
    return closedDrawingStream && RemoveConnectionId(connectionId); // return true only if the stream AND connection are both closed
}

void ServerManager::Start() {
    ServerConnectionServiceImpl connectionServiceImplementation(this);
    grpc::ServerBuilder builder;
    grpc::ServerContext context;
    // Listen on the given address using insecure credentials
    builder.AddListeningPort(m_TargetAddress, grpc::InsecureServerCredentials());
    builder.RegisterService(&connectionServiceImplementation);
    builder.RegisterService(drawingService_.get());

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
    if (m_ServerInstance) {
        m_ServerInstance->Shutdown(std::chrono::system_clock::now() + std::chrono::seconds(1));
    }
}

void ServerManager::Update() {

}

int ServerManager::AssignNewConnectionId(const std::string& peerAddress) {
    int newConnection = m_NumConnections;
    m_NumConnections++;
    return newConnection;
}

bool ServerManager::IsValidConnectionId(int connectionId) {
    std::lock_guard<std::mutex> lock(m_ServerConnectionMutex);
    return m_OpenClients.find(connectionId) != m_OpenClients.end();
}

bool ServerManager::RemoveConnectionId(int connectionId) {
    std::lock_guard<std::mutex> lock(m_ServerConnectionMutex);
    if (m_OpenClients.find(connectionId) != m_OpenClients.end()) {
        m_OpenClients.erase(connectionId);
        return true;
    }
    return false;
}

std::string ServerManager::ReserveObjectId() {
    // generate a random object id
    static std::random_device              rd;
    static std::mt19937                    gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

    std::stringstream ss;
    int i;
    ss << std::hex;
    for (i = 0; i < 8; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-4";
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    ss << dis2(gen);
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 12; i++) {
        ss << dis(gen);
    };
    return ss.str();
}
