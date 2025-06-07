#pragma once

#include <SFML/Graphics.hpp>

#include "WhiteboardStateMachine.h"

class IEventHandler {
public:

	enum class EventReturnType {
		kSuccess = 0,
		kAttemptCloseApplication = 1,
		kAttemptConnection = 2,
		kAttemptDisconnection = 3
	};

	virtual ~IEventHandler() = default;
	virtual IEventHandler::EventReturnType HandleEvent(const sf::Event& event, WhiteboardStateMachine::DrawTool& currentTool) = 0;
	virtual void SetConnectionId(int connectionId) {};
};