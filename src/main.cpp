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

#include "IDrawingManager.h"
#include "IEventHandler.h"
#include "IServerManager.h"

#include "DrawingManager.h"
#include "HomePageEventHandler.h"
#include "ServerManager.h"
#include "UIRenderer.h"
#include "WhiteboardPageEventHandler.h"

int main() {
    std::string server_address("localhost:50052");
    std::shared_ptr<IServerManager> serverManager = std::make_shared<ServerManager>(server_address);
    // Start the gRPC server in a new thread
    std::thread server_thread(&IServerManager::Start, serverManager);

    // Give the server a moment to start up.
    // In a real app, you'd use a more robust synchronization mechanism.
    std::cout << "[Main] Waiting a moment for the server to start..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    sf::RenderWindow window;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.create(sf::VideoMode({ 1024, 768 }, desktop.bitsPerPixel), "SFML window");

    sf::Font font;
    if (!font.openFromFile("../assets/fonts/KodeMono-VariableFont_wght.ttf")) {
        std::cerr << "Error loading font\n";
        return EXIT_FAILURE;
    }

    std::shared_ptr<UIRenderer> renderer = std::make_unique<UIRenderer>(window, font);
    std::shared_ptr<IDrawingManager> drawingManager = std::make_shared<DrawingManager>(serverManager);
    std::shared_ptr<IEventHandler> homePageEventHandler = std::make_shared<HomePageEventHandler>(window, serverManager);
    std::shared_ptr<IEventHandler> whiteboardPageEventHandler = std::make_shared<WhiteboardPageEventHandler>(window, serverManager, drawingManager);
    WhiteboardStateMachine::AppState currentState = WhiteboardStateMachine::AppState::kHome;
    WhiteboardStateMachine::DrawTool currentTool = WhiteboardStateMachine::DrawTool::kMarker;
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (currentState == WhiteboardStateMachine::AppState::kHome) {
                homePageEventHandler->handleEvent(*event, currentState, currentTool);
            }
            else if (currentState == WhiteboardStateMachine::AppState::kWhiteboard) {
                whiteboardPageEventHandler->handleEvent(*event, currentState, currentTool);
            }
        }
        renderer->clear();
        if (currentState == WhiteboardStateMachine::AppState::kHome) {
            renderer->renderHomeScreen();
        }
        else {
            std::vector<WhiteboardStateMachine::Line> lines = std::vector<WhiteboardStateMachine::Line>();
            std::vector<WhiteboardStateMachine::Rectangle> rectangles = std::vector<WhiteboardStateMachine::Rectangle>();
            WhiteboardStateMachine::Line currentLine = WhiteboardStateMachine::Line();
            WhiteboardStateMachine::Rectangle currentRect = WhiteboardStateMachine::Rectangle();
            renderer->renderWhiteboard(lines, rectangles, currentLine, currentRect, false, false, currentTool);
        }
        renderer->display();
    }

    serverManager->Stop();
    // Wait for the server thread to complete its execution
    if (server_thread.joinable()) {
        server_thread.join();
    }

    return 0;
}
