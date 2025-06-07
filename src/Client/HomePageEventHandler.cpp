#include "HomePageEventHandler.h"

#include <grpcpp/server.h>

HomePageEventHandler::HomePageEventHandler(sf::RenderWindow& window) : m_RenderWindow(window) {}
	
IEventHandler::EventReturnType HomePageEventHandler::HandleEvent(const sf::Event& event, WhiteboardStateMachine::DrawTool& currentTool) {
    if (event.is<sf::Event::Closed>()) {
        return IEventHandler::EventReturnType::kAttemptCloseApplication;
    }
    // Mouse button pressed event
    if (const sf::Event::MouseButtonPressed* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePosition = sf::Vector2f(mouseButtonPressed->position.x, mouseButtonPressed->position.y);
            // crude test to see if mouse position is on the button, update to be more dynamic
            sf::FloatRect connectButtonArea = sf::FloatRect(sf::Vector2f(m_RenderWindow.getSize().x / 2 - kButtonWidth / 2, m_RenderWindow.getSize().y / 2),
                sf::Vector2f(kButtonWidth, kButtonHeight));
            if (connectButtonArea.contains(mousePosition)) {
                return IEventHandler::EventReturnType::kAttemptConnection;
            }
            sf::FloatRect exitButtonArea = sf::FloatRect(sf::Vector2f(m_RenderWindow.getSize().x / 2 - kButtonWidth / 2, m_RenderWindow.getSize().y / 1.6),
                sf::Vector2f(kButtonWidth, kButtonHeight));
            if (exitButtonArea.contains(mousePosition)) {
                return IEventHandler::EventReturnType::kAttemptCloseApplication;
            }
        }
    }
    return IEventHandler::EventReturnType::kSuccess;
}
