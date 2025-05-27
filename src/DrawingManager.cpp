#include "DrawingManager.h"

DrawingManager::DrawingManager(std::shared_ptr<IServerManager> serverManager) : m_ServerManager(serverManager) {
	m_IsDrawing = false;
	m_IsRectStarted = false;
}

void DrawingManager::startDrawing(sf::Vector2f position, WhiteboardStateMachine::DrawTool tool) {

}

void DrawingManager::updateDrawing(sf::Vector2f position, WhiteboardStateMachine::DrawTool tool) {

}

void DrawingManager::endDrawing(WhiteboardStateMachine::DrawTool tool) {

}

void DrawingManager::eraseAt(sf::Vector2f position) {

}

void DrawingManager::clear() {

}
