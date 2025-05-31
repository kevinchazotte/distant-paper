#include "DrawingManager.h"

#include "../Utilities/DrawingTypeSerializationConverterUtil.h"
#include "../Utilities/GeometricIntersectionUtil.h"

DrawingManager::DrawingManager(std::shared_ptr<IServerConnectionManager> serverConnectionManager) : m_ServerConnectionManager(serverConnectionManager) {
    m_IsDrawing = false;
    m_CurrentLine = Whiteboard::Line();
    m_CurrentRectangle = Whiteboard::Rectangle();
    m_CursorCircle = sf::CircleShape(kEraserSize);
    m_CursorCircle.setOutlineColor(sf::Color::Black);
    m_CursorCircle.setOutlineThickness(1);
}

DrawingManager::~DrawingManager() {
    this->Clear();
}

void DrawingManager::StartDrawing(sf::Vector2f position, WhiteboardStateMachine::DrawTool tool) {
    if (m_IsDrawing) {
        return;
    }

    m_IsDrawing = true;
    if (tool == WhiteboardStateMachine::DrawTool::kMarker) {
        Whiteboard::Point* start = m_CurrentLine.mutable_start();
        *start = DrawingTypeSerializationConverterUtil::ToPoint(position, sf::Color::Black);
    }
    else if (tool == WhiteboardStateMachine::DrawTool::kRectangle) {
        Whiteboard::Point* start = m_CurrentRectangle.mutable_start();
        *start = DrawingTypeSerializationConverterUtil::ToPoint(position, sf::Color::Black);
    }
    else if (tool == WhiteboardStateMachine::DrawTool::kEraser) {
        m_CursorCircle.setPosition(position - m_CursorCircle.getGeometricCenter());
        EraseAt(position);
    }
}

void DrawingManager::UpdateDrawing(sf::Vector2f position, WhiteboardStateMachine::DrawTool tool) {
    if (tool == WhiteboardStateMachine::DrawTool::kMarker) {
        if (m_CurrentLine.has_end()) {
            Whiteboard::Point* end = m_CurrentLine.mutable_end();
            end->set_x(position.x);
            end->set_y(position.y);
        }
        else {
            Whiteboard::Point* end = m_CurrentLine.mutable_end();
            *end = DrawingTypeSerializationConverterUtil::ToPoint(position, sf::Color::Black);
        }
    }
    else if (tool == WhiteboardStateMachine::DrawTool::kRectangle) {
        if (m_CurrentRectangle.has_end()) {
            Whiteboard::Point* end = m_CurrentRectangle.mutable_end();
            end->set_x(position.x);
            end->set_y(position.y);
        }
        else {
            Whiteboard::Point* end = m_CurrentRectangle.mutable_end();
            *end = DrawingTypeSerializationConverterUtil::ToPoint(position, sf::Color::Black);
        }
    }
    else if (tool == WhiteboardStateMachine::DrawTool::kEraser) {
        m_CursorCircle.setPosition(position - m_CursorCircle.getGeometricCenter());
        EraseAt(position);
    }
}

void DrawingManager::EndDrawingAndUpdate(WhiteboardStateMachine::DrawTool tool) {
    if (!m_IsDrawing) {
        return;
    }

    m_IsDrawing = false;
    if (tool == WhiteboardStateMachine::DrawTool::kMarker) {
        m_Lines.push_back(m_CurrentLine); // for now, local only updates. eventually, send to server
        m_CurrentLine.Clear();
    }
    else if (tool == WhiteboardStateMachine::DrawTool::kRectangle) {
        m_Rectangles.push_back(m_CurrentRectangle); // for now, local only updates. eventually, send to server
        m_CurrentRectangle.Clear();
    }
    else if (tool == WhiteboardStateMachine::DrawTool::kEraser) {
        m_CursorCircle.setPosition(sf::Vector2f(0, 0));
    }
}

void DrawingManager::StopDrawing(WhiteboardStateMachine::DrawTool tool) {
    if (!m_IsDrawing) {
        return;
    }

    m_IsDrawing = false;
    if (tool == WhiteboardStateMachine::DrawTool::kMarker) {
        m_CurrentLine.Clear();
    }
    else if (tool == WhiteboardStateMachine::DrawTool::kRectangle) {
        m_CurrentRectangle.Clear();
    }
    else if (tool == WhiteboardStateMachine::DrawTool::kEraser) {
        m_CursorCircle.setPosition(sf::Vector2f(0, 0));
    }
}

void DrawingManager::EraseAt(sf::Vector2f position) {
    // erase any intersecting lines
    for (std::vector<Whiteboard::Line>::iterator it = m_Lines.begin(); it < m_Lines.end();) {
        std::array<sf::Vertex, 2> vertices = DrawingTypeSerializationConverterUtil::ToSFMLVertices(*it);
        if (GeometricIntersectionUtil::CircleIntersectsLineSegment(vertices, position, kEraserSize)) {
            it = m_Lines.erase(it);
        }
        else {
            it++;
        }
    }

    // erase any intersecting rectangles
    for (std::vector<Whiteboard::Rectangle>::iterator it = m_Rectangles.begin(); it < m_Rectangles.end();) {
        sf::RectangleShape rectangleShape = DrawingTypeSerializationConverterUtil::ToSFMLRectangleShape(*it);
        if (GeometricIntersectionUtil::CircleIntersectsRectangle(rectangleShape, position, kEraserSize)) {
            it = m_Rectangles.erase(it);
        }
        else {
            it++;
        }
    }
}

void DrawingManager::Clear() {
    m_IsDrawing = false;
    m_CurrentLine.Clear();
    m_CurrentRectangle.Clear();
    for (Whiteboard::Line line : m_Lines) {
        line.Clear();
    }
    m_Lines.clear();
    for (Whiteboard::Rectangle rectangle : m_Rectangles) {
        rectangle.Clear();
    }
    m_Rectangles.clear();
}
