#include "ClientConnection.h"

ClientConnection::ClientConnection(int connectionId, grpc::ServerReaderWriter<Whiteboard::Drawing::StreamEvent, Whiteboard::Drawing::StreamEvent>* stream) : 
	m_ConnectionId(connectionId), m_Stream(stream) {

}

int ClientConnection::GetConnectionId() {
	return m_ConnectionId;
}

void ClientConnection::SendEvent(const Whiteboard::Drawing::StreamEvent& event) {
	std::lock_guard<std::mutex> lock(m_ClientMutex);
	if (m_Stream) {
		m_Stream->Write(event);
	}
}
