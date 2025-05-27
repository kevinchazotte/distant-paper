#pragma once

#include <SFML/Graphics.hpp>

#include "WhiteboardStateMachine.h"

class IEventHandler {
public:
	virtual ~IEventHandler() = default;
	virtual void handleEvent(const sf::Event& event, WhiteboardStateMachine::AppState& currentState, WhiteboardStateMachine::DrawTool& currentTool) = 0;
};