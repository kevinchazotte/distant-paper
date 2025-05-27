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

    struct DrawPoint {
        sf::Vector2f position;
        sf::Color color;
    };

    struct Line {
        std::vector<DrawPoint> points;
    };

    struct Rectangle {
        sf::Vector2f start;
        sf::Vector2f end;
        bool isComplete;
    };
}
