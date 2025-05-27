#pragma once

#include "IEventHandler.h"

#include <memory>

#include "IDrawingManager.h"
#include "IServerManager.h"

class WhiteboardPageEventHandler : public IEventHandler {
public:
	WhiteboardPageEventHandler(sf::RenderWindow& window, std::shared_ptr<IServerManager> serverManager, std::shared_ptr<IDrawingManager> drawingManager);
	virtual ~WhiteboardPageEventHandler() = default;

	void handleEvent(const sf::Event& event, WhiteboardStateMachine::AppState& currentState, WhiteboardStateMachine::DrawTool& currentTool) override;
private:
	void handleMenuClick(sf::Vector2f mousePosition, WhiteboardStateMachine::AppState& currentState, WhiteboardStateMachine::DrawTool& currentTool);

	sf::RenderWindow& m_RenderWindow;
	std::shared_ptr<IDrawingManager> m_DrawingManager;
	std::shared_ptr<IServerManager> m_ServerManager;

	static const int kMenuBarHeight = 60;
	static const int kButtonWidth = 120;
	static const int kButtonHeight = 40;
};