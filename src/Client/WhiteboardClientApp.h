#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "DrawingManager.h"
#include "IEventHandler.h"
#include "IRenderer.h"
#include "ServerConnectionManager.h"
#include "WhiteboardStateMachine.h"

class WhiteboardClientApp {
public:
	WhiteboardClientApp(std::shared_ptr<ServerConnectionManager> serverConnectionManager);
	~WhiteboardClientApp();

	void Run();

private:
	sf::RenderWindow m_RenderWindow;

	std::shared_ptr<ServerConnectionManager> m_ServerConnectionManager;
	std::shared_ptr<DrawingManager> m_DrawingManager;

	std::shared_ptr<IEventHandler> m_HomePageEventHandler;
	std::shared_ptr<IRenderer> m_Renderer;
	std::shared_ptr<IEventHandler> m_WhiteboardPageEventHandler;

	WhiteboardStateMachine::AppState m_CurrentState;
	WhiteboardStateMachine::DrawTool m_CurrentTool;
};
