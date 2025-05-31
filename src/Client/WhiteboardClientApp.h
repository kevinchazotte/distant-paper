#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "IRenderer.h"
#include "IDrawingManager.h"
#include "IServerConnectionManager.h"
#include "IEventHandler.h"
#include "WhiteboardStateMachine.h"

class WhiteboardClientApp {
public:
	WhiteboardClientApp(std::shared_ptr<IServerConnectionManager> serverConnectionManager);
	~WhiteboardClientApp();

	void Run();

private:
	sf::RenderWindow m_RenderWindow;

	std::shared_ptr<IDrawingManager> m_DrawingManager;
	std::shared_ptr<IEventHandler> m_HomePageEventHandler;
	std::shared_ptr<IRenderer> m_Renderer;
	std::shared_ptr<IServerConnectionManager> m_ServerConnectionManager;
	std::shared_ptr<IEventHandler> m_WhiteboardPageEventHandler;

	WhiteboardStateMachine::AppState m_CurrentState;
	WhiteboardStateMachine::DrawTool m_CurrentTool;

	int m_ServerConnectionId;
};
