#pragma once

#include <SFML/Graphics.hpp>

#include "WhiteboardStateMachine.h"

class IEventHandler {
public:
	virtual ~IEventHandler() = default;
	virtual int HandleEvent(const sf::Event& event, WhiteboardStateMachine::AppState& currentState, WhiteboardStateMachine::DrawTool& currentTool) = 0;
	virtual void SetConnectionId(int connectionId) {};
};