#include <string>
#include <memory>

#include "ServerConnectionManager.h"
#include "WhiteboardClientApp.h"

int main() {
    std::string server_address("localhost:50052");
    std::shared_ptr<IServerConnectionManager> serverConnectionManager = std::make_shared<ServerConnectionManager>(server_address);
    WhiteboardClientApp clientApp = WhiteboardClientApp(serverConnectionManager);
    clientApp.Run();
    return 0;
}
