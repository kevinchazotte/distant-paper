#include "WhiteboardPageEventHandler.h"

WhiteboardPageEventHandler::WhiteboardPageEventHandler(sf::RenderWindow& window, std::shared_ptr<IDrawingManager> drawingManager) :
    m_RenderWindow(window), m_DrawingManager(drawingManager) {
}

IEventHandler::EventReturnType WhiteboardPageEventHandler::HandleEvent(const sf::Event& event, WhiteboardStateMachine::DrawTool& currentTool) {
    if (event.is<sf::Event::Closed>()) {
        return IEventHandler::EventReturnType::kAttemptCloseApplication;
    }
    if (const sf::Event::MouseButtonPressed* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePosition = sf::Vector2f(mouseButtonPressed->position.x, mouseButtonPressed->position.y);
            // crude test to see if mouse position is on the menu bar, update to be more dynamic
            sf::FloatRect menuBarArea = sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(m_RenderWindow.getSize().x, kMenuBarHeight + 2));
            if (menuBarArea.contains(mousePosition)) {
                return HandleMenuClick(mousePosition, currentTool);
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
        }
        else {
            if (m_DrawingManager->IsDrawing()) {
                m_DrawingManager->UpdateDrawing(mousePosition, currentTool);
            }
        }
    }
    return IEventHandler::EventReturnType::kSuccess;
}

IEventHandler::EventReturnType WhiteboardPageEventHandler::HandleMenuClick(sf::Vector2f mousePosition, WhiteboardStateMachine::DrawTool& currentTool) {
    // crude test to see if mouse position is on a menu button, update to be more dynamic
    // temporary values for visualizing
    int buttonX = 10;
    int buttonY = 10;
    int spacing = kButtonWidth + 10;

    sf::FloatRect homeButtonArea = sf::FloatRect(sf::Vector2f(buttonX, buttonY), sf::Vector2f(kButtonWidth, kButtonHeight));
    if (homeButtonArea.contains(mousePosition)) {
        return IEventHandler::EventReturnType::kAttemptDisconnection;
    }
    buttonX += spacing;
    sf::FloatRect markerButtonArea = sf::FloatRect(sf::Vector2f(buttonX, buttonY), sf::Vector2f(kButtonWidth, kButtonHeight));
    if (markerButtonArea.contains(mousePosition)) {
        m_DrawingManager->StopDrawing(currentTool);
        if (currentTool != WhiteboardStateMachine::DrawTool::kMarker) {
            currentTool = WhiteboardStateMachine::DrawTool::kMarker;
        }
        return IEventHandler::EventReturnType::kSuccess;
    }
    buttonX += spacing;
    sf::FloatRect rectangleButtonArea = sf::FloatRect(sf::Vector2f(buttonX, buttonY), sf::Vector2f(kButtonWidth, kButtonHeight));
    if (rectangleButtonArea.contains(mousePosition)) {
        m_DrawingManager->StopDrawing(currentTool);
        if (currentTool != WhiteboardStateMachine::DrawTool::kRectangle) {
            currentTool = WhiteboardStateMachine::DrawTool::kRectangle;
        }
        return IEventHandler::EventReturnType::kSuccess;
    }
    buttonX += spacing;
    sf::FloatRect eraserButtonArea = sf::FloatRect(sf::Vector2f(buttonX, buttonY), sf::Vector2f(kButtonWidth, kButtonHeight));
    if (eraserButtonArea.contains(mousePosition)) {
        m_DrawingManager->StopDrawing(currentTool);
        if (currentTool != WhiteboardStateMachine::DrawTool::kEraser) {
            currentTool = WhiteboardStateMachine::DrawTool::kEraser;
        }
        return IEventHandler::EventReturnType::kSuccess;
    }
    return IEventHandler::EventReturnType::kSuccess;
}
