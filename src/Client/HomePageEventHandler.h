#pragma once

#include "IEventHandler.h"

#include <memory>

#include <grpcpp/server.h>

#include "ServerConnectionManager.h"

class HomePageEventHandler : public IEventHandler {
public:
	HomePageEventHandler(sf::RenderWindow& window);
	virtual ~HomePageEventHandler() = default;

	EventReturnType HandleEvent(const sf::Event& event, WhiteboardStateMachine::DrawTool& currentTool) override;

private:
	sf::RenderWindow& m_RenderWindow;

	static const int kButtonWidth = 120;
	static const int kButtonHeight = 40;
};