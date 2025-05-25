#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

namespace WhiteboardStateMachine {
	enum class AppState {
        HOME,
        WHITEBOARD
    };

    enum class DrawTool {
        MARKER,
        RECTANGLE,
        ERASER
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
