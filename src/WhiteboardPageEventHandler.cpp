#include "WhiteboardPageEventHandler.h"

WhiteboardPageEventHandler::WhiteboardPageEventHandler(sf::RenderWindow& window, std::shared_ptr<IServerManager> serverManager, std::shared_ptr<IDrawingManager> drawingManager) :
    m_RenderWindow(window), m_ServerManager(serverManager), m_DrawingManager(drawingManager) {
}

int WhiteboardPageEventHandler::HandleEvent(const sf::Event& event, WhiteboardStateMachine::AppState& currentState, WhiteboardStateMachine::DrawTool& currentTool) {
    if (event.is<sf::Event::Closed>()) {
        m_RenderWindow.close();
    }
    else if (const sf::Event::MouseButtonPressed* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePosition = sf::Vector2f(mouseButtonPressed->position.x, mouseButtonPressed->position.y);
            // crude test to see if mouse position is on the menu bar, update to be more dynamic
            sf::FloatRect menuBarArea = sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(m_RenderWindow.getSize().x, kMenuBarHeight + 2));
            if (menuBarArea.contains(mousePosition)) {
                return HandleMenuClick(mousePosition, currentState, currentTool);
            }
            else {
                if (m_DrawingManager->IsDrawing()) {
                    m_DrawingManager->EndDrawingAndUpdate(currentTool);
                }
                else {
                    m_DrawingManager->StartDrawing(mousePosition, currentTool);
                }
            }
        }
        else if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
            if (m_DrawingManager->IsDrawing()) {
                m_DrawingManager->StopDrawing(currentTool);
            }
        }
    }
    else if (const sf::Event::MouseButtonReleased* mouseButtonReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
            if (m_DrawingManager->IsDrawing() && currentTool == WhiteboardStateMachine::DrawTool::kEraser) {
                m_DrawingManager->EndDrawingAndUpdate(currentTool);
            }
        }
    }
    else if (const sf::Event::MouseMoved* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        sf::Vector2f mousePosition = sf::Vector2f(mouseMoved->position.x, mouseMoved->position.y);
        // crude test to see if mouse position is on the menu bar, update to be more dynamic
        sf::FloatRect menuBarArea = sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(m_RenderWindow.getSize().x, kMenuBarHeight + 2));
        if (menuBarArea.contains(mousePosition)) {
            if (m_DrawingManager->IsDrawing() && currentTool == WhiteboardStateMachine::DrawTool::kEraser) {
                m_DrawingManager->EndDrawingAndUpdate(currentTool);
            }
            return -1; // don't update shape if on the menu bar
        }
        else {
            if (m_DrawingManager->IsDrawing()) {
                m_DrawingManager->UpdateDrawing(mousePosition, currentTool);
            }
        }
    }
    return -1;
}

int WhiteboardPageEventHandler::HandleMenuClick(sf::Vector2f mousePosition, WhiteboardStateMachine::AppState& currentState, WhiteboardStateMachine::DrawTool& currentTool) {
    // crude test to see if mouse position is on a menu button, update to be more dynamic
    // temporary values for visualizing
    int buttonX = 10;
    int buttonY = 10;
    int spacing = kButtonWidth + 10;

    sf::FloatRect homeButtonArea = sf::FloatRect(sf::Vector2f(buttonX, buttonY), sf::Vector2f(kButtonWidth, kButtonHeight));
    if (homeButtonArea.contains(mousePosition)) {
        bool success = m_ServerManager->Disconnect(m_ConnectionId); // once connection completes, update signal to emit new app state
        if (success) {
            m_DrawingManager->Clear();
            currentState = WhiteboardStateMachine::AppState::kHome; // for now, manually update app state
            return 0;
        }
        else {
            return -1;
        }
    }
    buttonX += spacing;
    sf::FloatRect markerButtonArea = sf::FloatRect(sf::Vector2f(buttonX, buttonY), sf::Vector2f(kButtonWidth, kButtonHeight));
    if (markerButtonArea.contains(mousePosition)) {
        m_DrawingManager->StopDrawing(currentTool);
        if (currentTool != WhiteboardStateMachine::DrawTool::kMarker) {
            currentTool = WhiteboardStateMachine::DrawTool::kMarker;
        }
        return -1;
    }
    buttonX += spacing;
    sf::FloatRect rectangleButtonArea = sf::FloatRect(sf::Vector2f(buttonX, buttonY), sf::Vector2f(kButtonWidth, kButtonHeight));
    if (rectangleButtonArea.contains(mousePosition)) {
        m_DrawingManager->StopDrawing(currentTool);
        if (currentTool != WhiteboardStateMachine::DrawTool::kRectangle) {
            currentTool = WhiteboardStateMachine::DrawTool::kRectangle;
        }
        return -1;
    }
    buttonX += spacing;
    sf::FloatRect eraserButtonArea = sf::FloatRect(sf::Vector2f(buttonX, buttonY), sf::Vector2f(kButtonWidth, kButtonHeight));
    if (eraserButtonArea.contains(mousePosition)) {
        m_DrawingManager->StopDrawing(currentTool);
        if (currentTool != WhiteboardStateMachine::DrawTool::kEraser) {
            currentTool = WhiteboardStateMachine::DrawTool::kEraser;
        }
        return -1;
    }
    return -1;
}

void WhiteboardPageEventHandler::SetConnectionId(int connectionId) {
    m_ConnectionId = connectionId;
}
