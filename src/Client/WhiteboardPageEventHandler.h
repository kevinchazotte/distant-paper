#pragma once

#include "IEventHandler.h"

#include <memory>

#include "IDrawingManager.h"
#include "ServerConnectionManager.h"

class WhiteboardPageEventHandler : public IEventHandler {
public:
	WhiteboardPageEventHandler(sf::RenderWindow& window, std::shared_ptr<IDrawingManager> drawingManager);
	virtual ~WhiteboardPageEventHandler() = default;

	IEventHandler::EventReturnType HandleEvent(const sf::Event& event, WhiteboardStateMachine::DrawTool& currentTool) override;

private:
	IEventHandler::EventReturnType HandleMenuClick(sf::Vector2f mousePosition, WhiteboardStateMachine::DrawTool& currentTool);

	sf::RenderWindow& m_RenderWindow;
	std::shared_ptr<IDrawingManager> m_DrawingManager;

	static const int kMenuBarHeight = 60;
	static const int kButtonWidth = 120;
	static const int kButtonHeight = 40;
};
