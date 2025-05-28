#include "ServerManager.h"

// gRPC headers
#include <grpcpp/grpcpp.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
// No security headers needed for insecure credentials

// Generated protobuf and gRPC files
#include "greeter.pb.h"
#include "greeter.grpc.pb.h"

// Server implementation
class GreeterServiceImpl final : public helloworld::Greeter::Service {
    grpc::Status SayHello(grpc::ServerContext* context,
        const helloworld::HelloRequest* request,
        helloworld::HelloReply* reply) override {
        std::string prefix("Hello (via insecure gRPC), ");
        reply->set_message(prefix + request->name() + "!");
        std::cout << "[Server Thread] Responded to: " << request->name() << std::endl;
        return grpc::Status::OK;
    }
};

ServerManager::ServerManager(const std::string targetAddress) : m_TargetAddress(targetAddress) {
    
}

void ServerManager::Connect() {
    std::cout << "[Client] Opening a connection with id " << std::to_string(m_NumConnections) << " to " << m_TargetAddress << std::endl;

    // Create an insecure channel
    grpc::ChannelArguments channel_args; // Can be empty for simple cases
    std::shared_ptr<grpc::Channel> channel = grpc::CreateCustomChannel(m_TargetAddress, grpc::InsecureChannelCredentials(), channel_args);
    m_OpenClients.insert(std::make_pair(m_NumConnections, channel));

    std::cout << "[Client] Testing connection with client stub..." << std::endl;
    std::unique_ptr<helloworld::Greeter::Stub> stub = helloworld::Greeter::NewStub(channel);

    helloworld::HelloRequest request;
    request.set_name("Simpler gRPC Test");

    helloworld::HelloReply reply;
    grpc::ClientContext context;

    // Optional: Set a deadline for the RPC
    std::chrono::system_clock::time_point deadline =
        std::chrono::system_clock::now() + std::chrono::seconds(5);
    context.set_deadline(deadline);

    std::cout << "[Client] Sending SayHello RPC..." << std::endl;
    grpc::Status status = stub->SayHello(&context, request, &reply);

    if (status.ok()) {
        std::cout << "[Client] Received reply: '" << reply.message() << "'" << std::endl;
        m_NumConnections++;
    }
    else {
        std::cerr << "[Client] RPC failed with code " << status.error_code()
            << ": " << status.error_message() << std::endl;
    }
}

void ServerManager::Disconnect() {
    
}

void ServerManager::SendLine(const WhiteboardStateMachine::Line& line) {

}

void ServerManager::SendRectangle(const WhiteboardStateMachine::Rectangle& rect) {

}

void ServerManager::SendErase(sf::Vector2f position) {

}

void ServerManager::Start() {
    GreeterServiceImpl service_impl;
    grpc::ServerBuilder builder;

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
