#include "WhiteboardClientApp.h"

#include "DrawingManager.h"
#include "HomePageEventHandler.h"
#include "ServerConnectionManager.h"
#include "UIRenderer.h"
#include "WhiteboardPageEventHandler.h"

WhiteboardClientApp::WhiteboardClientApp(std::shared_ptr<ServerConnectionManager> serverConnectionManager) {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    m_RenderWindow.create(sf::VideoMode(sf::Vector2u(1024, 768), desktop.bitsPerPixel), "Distant-Paper");

    sf::Font font;
    if (!font.openFromFile("./assets/fonts/KodeMono-VariableFont_wght.ttf") && // install build assets location
        !font.openFromFile("../../assets/fonts/KodeMono-VariableFont_wght.ttf") && // VS working directory assets location
        !font.openFromFile("../../../assets/fonts/KodeMono-VariableFont_wght.ttf")) { // binary directory assets location
        std::cerr << "Error loading font\n";
        return;
    }

    m_ServerConnectionManager = serverConnectionManager;
    m_DrawingManager = std::make_shared<DrawingManager>(m_ServerConnectionManager);
    m_Renderer = std::make_unique<UIRenderer>(m_RenderWindow, font);
    m_HomePageEventHandler = std::make_shared<HomePageEventHandler>(m_RenderWindow);
    m_WhiteboardPageEventHandler = std::make_shared<WhiteboardPageEventHandler>(m_RenderWindow, m_DrawingManager);

    m_CurrentState = WhiteboardStateMachine::AppState::kHome;
    m_CurrentTool = WhiteboardStateMachine::DrawTool::kMarker;
}

WhiteboardClientApp::~WhiteboardClientApp() {

}

void WhiteboardClientApp::CloseWindow() {
    m_RenderWindow.close();
}

void WhiteboardClientApp::Run() {
    while (m_RenderWindow.isOpen()) {
        while (const std::optional event = m_RenderWindow.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                this->CloseWindow();
            }
            if (m_CurrentState == WhiteboardStateMachine::AppState::kHome) {
                IEventHandler::EventReturnType ret = m_HomePageEventHandler->HandleEvent(*event, m_CurrentTool);
                if (ret == IEventHandler::EventReturnType::kAttemptConnection) {
                    if (!m_ServerConnectionManager->Connect()) {
                        std::cerr << "[Client] Failed to establish connection." << std::endl;
                        continue; // TODO: either update the home page UI or display some error text
                    }

                    m_CurrentState = WhiteboardStateMachine::AppState::kWhiteboard;
                    std::cout << "[Client] Established connection with id " << std::to_string(m_ServerConnectionManager->GetConnectionId()) << std::endl;
                    if (!m_ServerConnectionManager->OpenSubscriberStream(m_DrawingManager.get())) {
                        std::cerr << "[Client] Failed to subscribe to server event stream." << std::endl;
                    }
                    continue;
                }
                else if (ret == IEventHandler::EventReturnType::kAttemptCloseApplication) {
                    this->CloseWindow();
                }
            }
            else if (m_CurrentState == WhiteboardStateMachine::AppState::kWhiteboard) {
                IEventHandler::EventReturnType ret = m_WhiteboardPageEventHandler->HandleEvent(*event, m_CurrentTool);
                if (ret == IEventHandler::EventReturnType::kAttemptDisconnection) {
                    if (!m_ServerConnectionManager->Disconnect()) {
                        std::cerr << "[Client] Failed to disconnect from server." << std::endl;
                        continue; // TODO: either update whiteboard page UI or display some error text
                    }
                    m_CurrentState = WhiteboardStateMachine::AppState::kHome;
                    m_DrawingManager->Clear();
                    continue;
                }
                else if (ret == IEventHandler::EventReturnType::kAttemptCloseApplication) {
                    this->CloseWindow();
                }
            }
        }
        m_Renderer->Clear();
        if (m_CurrentState == WhiteboardStateMachine::AppState::kHome) {
            m_Renderer->RenderHomeScreen();
        }
        else {
            m_Renderer->RenderWhiteboard(m_DrawingManager->GetDrawables(),
                m_DrawingManager->GetCurrentLine(), m_DrawingManager->GetCurrentRectangle(), m_DrawingManager->GetCursorCircle(),
                m_DrawingManager->IsDrawing(), m_CurrentTool);
        }
        m_Renderer->Display();
    }
}
