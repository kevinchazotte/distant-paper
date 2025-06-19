#include <string>
#include <memory>

#include "ServerConnectionManager.h"
#include "WhiteboardClientApp.h"

int main(int argc, char** argv) {
    std::string server_address("localhost:50052");
    if (argc > 1) {
        server_address = argv[1];
    }
    std::shared_ptr<ServerConnectionManager> serverConnectionManager = std::make_shared<ServerConnectionManager>(server_address);
    WhiteboardClientApp clientApp = WhiteboardClientApp(serverConnectionManager);
    clientApp.Run();
    return 0;
}
