#pragma once

#include <memory>
#include <mutex>
#include <unordered_map>
#include <unordered_set>

#include <grpcpp/server.h>

#include "drawing_service.grpc.pb.h"
#include "drawing_service.pb.h"
#include "drawing_types.grpc.pb.h"
#include "drawing_types.pb.h"

#include "ClientConnection.h"

class ServerManager;

class DrawingServiceImpl final : public Whiteboard::Drawing::DrawingService::Service {
public:
	explicit DrawingServiceImpl(ServerManager* serverManager) : serverManager_(serverManager) {
		m_ActiveStreams = std::unordered_set<std::shared_ptr<ClientConnection>>();
	}

	grpc::Status OpenDrawingStream(grpc::ServerContext* context, 
		grpc::ServerReaderWriter<Whiteboard::Drawing::StreamEvent, Whiteboard::Drawing::StreamEvent>* stream);

	bool CloseDrawingStream(int connectionId);

	void BroadcastDrawable(const Whiteboard::Types::Drawable& drawable);
	void BroadcastErase(const Whiteboard::Types::Drawable& drawable);

private:
	ServerManager* serverManager_;

	std::unordered_set<std::shared_ptr<ClientConnection>> m_ActiveStreams;
	std::mutex m_StreamWriterMutex;

	std::unordered_map<std::string, Whiteboard::Types::Drawable> m_DrawnObjects;
	std::mutex m_ObjectWriterMutex;

	void SendDrawablesToNewStream(std::shared_ptr<ClientConnection> clientStream);
};