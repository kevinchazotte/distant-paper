#pragma once

#include "IEventHandler.h"

#include <memory>

#include "IDrawingManager.h"
#include "IServerConnectionManager.h"

class WhiteboardPageEventHandler : public IEventHandler {
public:
	WhiteboardPageEventHandler(sf::RenderWindow& window,
		std::shared_ptr<IServerConnectionManager> serverConnectionManager, std::shared_ptr<IDrawingManager> drawingManager);
	virtual ~WhiteboardPageEventHandler() = default;

	int HandleEvent(const sf::Event& event, WhiteboardStateMachine::AppState& currentState, WhiteboardStateMachine::DrawTool& currentTool) override;

	void SetConnectionId(int connectionId) override;
private:
	int HandleMenuClick(sf::Vector2f mousePosition, WhiteboardStateMachine::AppState& currentState, WhiteboardStateMachine::DrawTool& currentTool);

	sf::RenderWindow& m_RenderWindow;
	std::shared_ptr<IDrawingManager> m_DrawingManager;
	std::shared_ptr<IServerConnectionManager> m_ServerConnectionManager;

	static const int kMenuBarHeight = 60;
	static const int kButtonWidth = 120;
	static const int kButtonHeight = 40;

	int m_ConnectionId;
};