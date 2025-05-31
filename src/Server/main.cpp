#include <iostream>
#include <string>
#include <thread>

#include "ServerManager.h"

int main(int argc, char** argv) {
    std::string server_address("localhost:50052");
    if (argc > 1) {
        server_address = argv[1];
    }
    ServerManager serverManager = ServerManager(server_address);
    // Start the gRPC server in a new thread
    std::thread server_thread(&ServerManager::Start, &serverManager);

    std::cout << "Server running at address " << server_address << ".Press Enter to stop..." << std::endl;
    std::cin.get();

    serverManager.Stop();
    if (server_thread.joinable()) {
        server_thread.join();
    }

    return 0;
}
