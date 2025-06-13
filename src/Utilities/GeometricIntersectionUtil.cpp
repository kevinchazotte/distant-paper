#include "GeometricIntersectionUtil.h"

#include <algorithm>
#include <cmath>

double distance(sf::Vector2f p1, sf::Vector2f p2) {
    return std::sqrt(std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2));
}

double dotProduct(sf::Vector2f p1, sf::Vector2f p2) {
    return p1.x * p2.x + p1.y * p2.y;
}

bool GeometricIntersectionUtil::CircleIntersectsLineSegment(std::array<sf::Vertex, 2> vertices, sf::Vector2f center, float radius) {
    return CircleIntersectsLineSegment(vertices[0].position, vertices[1].position, center, radius);
}

bool GeometricIntersectionUtil::CircleIntersectsLineSegment(sf::Vector2f start, sf::Vector2f end, sf::Vector2f center, float radius) {
    sf::Vector2f startToCircleCenter = center - start;
    sf::Vector2f lineSegment = end - start;
    double segmentProjectionRatio = dotProduct(startToCircleCenter, lineSegment) / dotProduct(lineSegment, lineSegment);
    segmentProjectionRatio = std::clamp(segmentProjectionRatio, 0.0, 1.0);
    sf::Vector2f circleProjectionPoint = start + static_cast<float>(segmentProjectionRatio) * lineSegment;
    double distanceToLine = distance(center, circleProjectionPoint);
    return distanceToLine <= radius;
}

bool GeometricIntersectionUtil::CircleIntersectsRectangle(sf::RectangleShape rectangleShape, sf::Vector2f center, float radius) {
    sf::Vector2f topLeft = rectangleShape.getPosition();
    sf::Vector2f bottomRight = topLeft + rectangleShape.getSize();
    sf::Vector2f topRight = sf::Vector2f(bottomRight.x, topLeft.y);
    if (CircleIntersectsLineSegment(topLeft, topRight, center, radius)) {
        return true;
    }
    if (CircleIntersectsLineSegment(topRight, bottomRight, center, radius)) {
        return true;
    }
    sf::Vector2f bottomLeft = sf::Vector2f(topLeft.x, bottomRight.y);
    if (CircleIntersectsLineSegment(topLeft, bottomLeft, center, radius)) {
        return true;
    }
    if (CircleIntersectsLineSegment(bottomLeft, bottomRight, center, radius)) {
        return true;
    }
    return false;
}
