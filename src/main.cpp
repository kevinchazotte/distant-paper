#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>

// SFML headers
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

// gRPC headers
#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
// No security headers needed for insecure credentials

// Generated protobuf and gRPC files
#include "greeter.pb.h"
#include "greeter.grpc.pb.h"

#include "IServerManager.h"
#include "ServerManager.h"
#include "WhiteboardClientApp.h"

int main() {
    std::string server_address("localhost:50052");
    std::shared_ptr<IServerManager> serverManager = std::make_shared<ServerManager>(server_address);
    // Start the gRPC server in a new thread
    std::thread server_thread(&IServerManager::Start, serverManager);

    // Give the server a moment to start up.
    // In a real app, you'd use a more robust synchronization mechanism.
    std::cout << "[Main] Waiting a moment for the server to start..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    WhiteboardClientApp clientApp = WhiteboardClientApp(serverManager);
    clientApp.Run();

    serverManager->Stop();
    // Wait for the server thread to complete its execution
    if (server_thread.joinable()) {
        server_thread.join();
    }

    return 0;
}
