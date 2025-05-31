#include "ServerConnectionManager.h"

#include <grpcpp/grpcpp.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include "greeter.pb.h"

ServerConnectionManager::ServerConnectionManager(const std::string& targetAddress) : m_TargetAddress(targetAddress) {
    m_ConnectionId = -1;
}

bool ServerConnectionManager::Connect() {
    std::cout << "[Client] Opening a connection to " << m_TargetAddress << std::endl;

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
    if (status.ok()) {
        m_ConnectionId = reply.connection();
        if (m_ConnectionId < 0) {
            std::cerr << "[Client] RPC failed with connection Id " << std::to_string(m_ConnectionId) << std::endl;
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
    return true;
}

void ServerConnectionManager::SendLine(const Whiteboard::Line& line) {

}

void ServerConnectionManager::SendRectangle(const Whiteboard::Rectangle& rect) {

}

void ServerConnectionManager::SendErase(sf::Vector2f position) {

}

void ServerConnectionManager::Update() {

}
