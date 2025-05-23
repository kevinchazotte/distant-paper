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

// Global server pointer to allow shutdown from main thread
std::unique_ptr<grpc::Server> server_instance;

void RunGrpcServer(const std::string& server_address) {
    GreeterServiceImpl service_impl;
    grpc::ServerBuilder builder;

    // Listen on the given address using insecure credentials
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service_impl);

    // Assemble and start the server
    server_instance = builder.BuildAndStart();
    if (server_instance) {
        std::cout << "[Server Thread] Server listening on " << server_address << std::endl;
        // Keep server running until Shutdown() is called from another thread
        server_instance->Wait();
        std::cout << "[Server Thread] Server shutdown complete." << std::endl;
    }
    else {
        std::cerr << "[Server Thread] Failed to start server on " << server_address << std::endl;
    }
}

void RunGrpcClient(const std::string& target_address) {
    std::cout << "[Client] Attempting to connect to " << target_address << std::endl;

    // Create an insecure channel
    grpc::ChannelArguments channel_args; // Can be empty for simple cases
    auto channel = grpc::CreateCustomChannel(target_address, grpc::InsecureChannelCredentials(), channel_args);

    std::cout << "[Client] Creating stub..." << std::endl;
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
    }
    else {
        std::cerr << "[Client] RPC failed with code " << status.error_code()
            << ": " << status.error_message() << std::endl;
    }
}

int main() {
    std::string server_address("localhost:50052"); // Use a different port if 50051 is busy

    // Start the gRPC server in a new thread
    std::thread server_thread(RunGrpcServer, server_address);

    // Give the server a moment to start up.
    // In a real app, you'd use a more robust synchronization mechanism.
    std::cout << "[Main] Waiting a moment for the server to start..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Run the gRPC client logic
    RunGrpcClient(server_address);

    // Shutdown the server
    if (server_instance) {
        std::cout << "[Main] Requesting server shutdown..." << std::endl;
        server_instance->Shutdown(std::chrono::system_clock::now() + std::chrono::seconds(1)); // Wait up to 1s for graceful shutdown
    }
    else {
        std::cerr << "[Main] Server instance was not found for shutdown." << std::endl;
    }

    // Wait for the server thread to complete its execution
    if (server_thread.joinable()) {
        server_thread.join();
    }

    std::cout << "[Main] Test sequence finished." << std::endl;

    // Create the main window
    sf::RenderWindow window;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.create(sf::VideoMode({1024, 768}, desktop.bitsPerPixel), "SFML window");

    sf::Font font;
    if (!font.openFromFile("../assets/fonts/KodeMono-VariableFont_wght.ttf")) {
        std::cerr << "Error loading font\n";
        return EXIT_FAILURE;
    }

    // Create a text object
    sf::Text text(font, "Hello, SFML UI!");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(10.f, 10.f)); // Position from top-left

    // Create a simple button (represented by a rectangle and text)
    sf::RectangleShape button(sf::Vector2f(150.f, 50.f));
    button.setPosition(sf::Vector2f(10.f, 50.f));
    button.setFillColor(sf::Color::Green);

    sf::Text buttonText(font, "Click Me");
    buttonText.setCharacterSize(20);
    buttonText.setFillColor(sf::Color::Blue);
    // Center text on button (approximate)
    sf::FloatRect buttonRect = button.getGlobalBounds();
    sf::FloatRect textRect = buttonText.getLocalBounds();
    buttonText.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f,
        textRect.position.y + textRect.size.y / 2.0f));
    buttonText.setPosition(sf::Vector2f(buttonRect.position.x + buttonRect.size.x / 2.0f,
        buttonRect.position.y + buttonRect.size.y / 2.0f));

    // Main loop: continues as long as the window is open
    while (window.isOpen()) {
        // Process events
        while (const std::optional event = window.pollEvent()) {
            // Close window: exit
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Mouse button pressed event
            if (const sf::Event::MouseButtonPressed* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    // Check if the click was inside the button
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        text.setString("Button Clicked!");
                        std::cout << "Button was clicked!" << std::endl;
                    }
                }
            }
        }

        // Clear screen
        window.clear(sf::Color::Black); // Fill background with black

        // Draw the text and button
        window.draw(text);
        window.draw(button);
        window.draw(buttonText);

        // Update the window
        window.display();
    }

    return 0;
}
