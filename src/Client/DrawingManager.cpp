#include "DrawingManager.h"

#include "../Utilities/DrawingTypeSerializationConverterUtil.h"
#include "../Utilities/GeometricIntersectionUtil.h"

DrawingManager::DrawingManager(std::shared_ptr<ServerConnectionManager> serverConnectionManager) : m_ServerConnectionManager(serverConnectionManager) {
    m_IsDrawing = false;
    m_IdCounter = 0;
    m_CurrentLine = Whiteboard::Types::Line();
    m_CurrentRectangle = Whiteboard::Types::Rectangle();
    m_CursorCircle = sf::CircleShape(kEraserSize);
    m_CursorCircle.setOutlineColor(sf::Color::Black);
    m_CursorCircle.setOutlineThickness(1);
}

DrawingManager::~DrawingManager() {
    this->Clear();
}

void DrawingManager::OnDrawableAdded(const Whiteboard::Types::Drawable& drawable) {
    std::lock_guard<std::mutex> lock(m_DrawablesMutex);
    std::string id = "";
    if (drawable.has_line()) {
        id = drawable.line().id();
    }
    else if (drawable.has_rectangle()) {
        id = drawable.rectangle().id();
    }

    if (id == "") {
        return;
    }
    m_Drawables[id] = drawable;
    m_IdCounter = std::stoi(id) + 1; // m_IdCounter should always be greater than the id of the last drawable
}

void DrawingManager::OnDrawableErased(const Whiteboard::Types::Drawable& drawable) {
    std::lock_guard<std::mutex> lock(m_DrawablesMutex);
    std::string id = "";
    if (drawable.has_line()) {
        id = drawable.line().id();
    }
    else if (drawable.has_rectangle()) {
        id = drawable.rectangle().id();
    }

    if (id == "") {
        return;
    }
    m_Drawables.erase(id);
}

void DrawingManager::OnListenerDisconnected(const grpc::Status& status) {
    std::cerr << "[Client] Listener disconnected with error code " << status.error_code() << ": " << status.error_message() << std::endl;
    return;
}

void DrawingManager::StartDrawing(sf::Vector2f position, WhiteboardStateMachine::DrawTool tool) {
    if (m_IsDrawing) {
        return;
    }

    m_IsDrawing = true;
    if (tool == WhiteboardStateMachine::DrawTool::kMarker) {
        Whiteboard::Types::Point* start = m_CurrentLine.mutable_start();
        *start = DrawingTypeSerializationConverterUtil::ToPoint(position, sf::Color::Black);
    }
    else if (tool == WhiteboardStateMachine::DrawTool::kRectangle) {
        Whiteboard::Types::Point* start = m_CurrentRectangle.mutable_start();
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
            Whiteboard::Types::Point* end = m_CurrentLine.mutable_end();
            end->set_x(position.x);
            end->set_y(position.y);
        }
        else {
            Whiteboard::Types::Point* end = m_CurrentLine.mutable_end();
            *end = DrawingTypeSerializationConverterUtil::ToPoint(position, sf::Color::Black);
        }
    }
    else if (tool == WhiteboardStateMachine::DrawTool::kRectangle) {
        if (m_CurrentRectangle.has_end()) {
            Whiteboard::Types::Point* end = m_CurrentRectangle.mutable_end();
            end->set_x(position.x);
            end->set_y(position.y);
        }
        else {
            Whiteboard::Types::Point* end = m_CurrentRectangle.mutable_end();
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
        m_CurrentLine.set_id(std::to_string(m_IdCounter++));
        Whiteboard::Types::Drawable drawable;
        *drawable.mutable_line() = m_CurrentLine;
        if (!m_ServerConnectionManager->SendDrawable(drawable)) {
            std::cerr << "[Client] Failed to send drawable with id " << m_CurrentLine.id() << std::endl;
        }
        m_CurrentLine.Clear();
    }
    else if (tool == WhiteboardStateMachine::DrawTool::kRectangle) {
        m_CurrentRectangle.set_id(std::to_string(m_IdCounter++));
        Whiteboard::Types::Drawable drawable;
        *drawable.mutable_rectangle() = m_CurrentRectangle;
        if (!m_ServerConnectionManager->SendDrawable(drawable)) {
            std::cerr << "[Client] Failed to send drawable with id " << m_CurrentRectangle.id() << std::endl;
        }
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
    std::lock_guard<std::mutex> lock(m_DrawablesMutex);
    // erase intersected drawable objects
    for (std::unordered_map<std::string, Whiteboard::Types::Drawable>::iterator it = m_Drawables.begin(); it != m_Drawables.end(); it++) {
        if (it->second.has_line()) {
            std::array<sf::Vertex, 2> vertices = DrawingTypeSerializationConverterUtil::ToSFMLVertices(it->second.line());
            if (GeometricIntersectionUtil::CircleIntersectsLineSegment(vertices, position, kEraserSize)) {
                if (!m_ServerConnectionManager->SendErase(it->second)) {
                    std::cerr << "[Client] Failed to send drawable with id " << m_CurrentLine.id() << std::endl;
                }
            }
        }
        else if (it->second.has_rectangle()) {
            sf::RectangleShape rectangleShape = DrawingTypeSerializationConverterUtil::ToSFMLRectangleShape(it->second.rectangle());
            if (GeometricIntersectionUtil::CircleIntersectsRectangle(rectangleShape, position, kEraserSize)) {
                if (!m_ServerConnectionManager->SendErase(it->second)) {
                    std::cerr << "[Client] Failed to send drawable with id " << m_CurrentRectangle.id() << std::endl;
                }
            }
        }
    }
}

void DrawingManager::Clear() {
    std::lock_guard<std::mutex> lock(m_DrawablesMutex);
    m_IsDrawing = false;
    m_CurrentLine.Clear();
    m_CurrentRectangle.Clear();
    for (std::pair<std::string, Whiteboard::Types::Drawable> pair : m_Drawables) {
        pair.second.Clear();
    }
    m_Drawables.clear();
}

std::unordered_map<std::string, Whiteboard::Types::Drawable> DrawingManager::GetDrawables() {
    std::lock_guard<std::mutex> lock(m_DrawablesMutex);
    return m_Drawables;
}
