#include "WhiteboardClientApp.h"

#include "DrawingManager.h"
#include "HomePageEventHandler.h"
#include "ServerConnectionManager.h"
#include "UIRenderer.h"
#include "WhiteboardPageEventHandler.h"

WhiteboardClientApp::WhiteboardClientApp(std::shared_ptr<IServerConnectionManager> serverConnectionManager) {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    m_RenderWindow.create(sf::VideoMode(sf::Vector2u(1024, 768), desktop.bitsPerPixel), "SFML window");

    sf::Font font;
    if (!font.openFromFile("../../assets/fonts/KodeMono-VariableFont_wght.ttf") && 
        !font.openFromFile("../../../assets/fonts/KodeMono-VariableFont_wght.ttf")) {
        std::cerr << "Error loading font\n";
        return;
    }

    m_ServerConnectionManager = serverConnectionManager;
    m_DrawingManager = std::make_shared<DrawingManager>(m_ServerConnectionManager);
    m_HomePageEventHandler = std::make_shared<HomePageEventHandler>(m_RenderWindow, m_ServerConnectionManager);
    m_Renderer = std::make_unique<UIRenderer>(m_RenderWindow, font);
    m_WhiteboardPageEventHandler = std::make_shared<WhiteboardPageEventHandler>(m_RenderWindow, m_ServerConnectionManager, m_DrawingManager);

    m_CurrentState = WhiteboardStateMachine::AppState::kHome;
    m_CurrentTool = WhiteboardStateMachine::DrawTool::kMarker;
    m_ServerConnectionId = -1;
}

WhiteboardClientApp::~WhiteboardClientApp() {

}

void WhiteboardClientApp::Run() {
    while (m_RenderWindow.isOpen()) {
        while (const std::optional event = m_RenderWindow.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                m_RenderWindow.close();
            }
            if (m_CurrentState == WhiteboardStateMachine::AppState::kHome) {
                int connectionId = m_HomePageEventHandler->HandleEvent(*event, m_CurrentState, m_CurrentTool);
                if (connectionId >= 0) {
                    m_ServerConnectionId = connectionId;
                    m_WhiteboardPageEventHandler->SetConnectionId(m_ServerConnectionId);
                    std::cout << "[Client] Established connection with id " << std::to_string(m_ServerConnectionId) << std::endl;
                }
            }
            else if (m_CurrentState == WhiteboardStateMachine::AppState::kWhiteboard) {
                int ret = m_WhiteboardPageEventHandler->HandleEvent(*event, m_CurrentState, m_CurrentTool);
                if (ret == 0) {
                    m_DrawingManager->Clear();
                }
            }
        }
        m_Renderer->Clear();
        if (m_CurrentState == WhiteboardStateMachine::AppState::kHome) {
            m_Renderer->RenderHomeScreen();
        }
        else {
            m_Renderer->RenderWhiteboard(m_DrawingManager->GetLines(), m_DrawingManager->GetRectangles(),
                m_DrawingManager->GetCurrentLine(), m_DrawingManager->GetCurrentRectangle(), m_DrawingManager->GetCursorCircle(),
                m_DrawingManager->IsDrawing(), m_CurrentTool);
        }
        m_Renderer->Display();
    }
}
