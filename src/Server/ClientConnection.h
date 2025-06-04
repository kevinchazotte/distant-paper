#pragma once

#include <mutex>

#include <grpcpp/server.h>

#include "drawing_service.grpc.pb.h"
#include "drawing_service.pb.h"

class ClientConnection {
public:
	ClientConnection(int connectionId, grpc::ServerReaderWriter<Whiteboard::Drawing::StreamEvent, Whiteboard::Drawing::StreamEvent>* stream);
	int GetConnectionId();
	void SendEvent(const Whiteboard::Drawing::StreamEvent& event);

private:
	const int m_ConnectionId;
	grpc::ServerReaderWriter<Whiteboard::Drawing::StreamEvent, Whiteboard::Drawing::StreamEvent>* m_Stream;
	std::mutex m_ClientMutex;
};