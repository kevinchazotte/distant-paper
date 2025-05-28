#include "DrawingManager.h"

DrawingManager::DrawingManager(std::shared_ptr<IServerManager> serverManager) : m_ServerManager(serverManager) {
	m_IsDrawing = false;
	m_IsRectStarted = false;
}

void DrawingManager::StartDrawing(sf::Vector2f position, WhiteboardStateMachine::DrawTool tool) {

}

void DrawingManager::UpdateDrawing(sf::Vector2f position, WhiteboardStateMachine::DrawTool tool) {

}

void DrawingManager::EndDrawing(WhiteboardStateMachine::DrawTool tool) {

}

void DrawingManager::EraseAt(sf::Vector2f position) {

}

void DrawingManager::Clear() {

}
