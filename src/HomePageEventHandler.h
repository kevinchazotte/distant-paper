#pragma once

#include "IEventHandler.h"

#include <memory>

#include "IServerManager.h"

class HomePageEventHandler : public IEventHandler {
public:
	HomePageEventHandler(sf::RenderWindow& window, std::shared_ptr<IServerManager> serverManager);
	virtual ~HomePageEventHandler() = default;

	void handleEvent(const sf::Event& event, WhiteboardStateMachine::AppState& currentState, WhiteboardStateMachine::DrawTool& currentTool) override;
private:
	sf::RenderWindow& m_RenderWindow;
	std::shared_ptr<IServerManager> m_ServerManager;

	static const int kButtonWidth = 120;
	static const int kButtonHeight = 40;
};