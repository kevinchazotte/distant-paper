#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

namespace WhiteboardStateMachine {
	enum class AppState {
        kHome,
        kWhiteboard
    };

    enum class DrawTool {
        kMarker,
        kRectangle,
        kEraser
    };
}
