#include "DrawingService.h"

#include "ServerManager.h"

grpc::Status DrawingServiceImpl::OpenDrawingStream(grpc::ServerContext* context,
	grpc::ServerReaderWriter<Whiteboard::Drawing::StreamEvent, Whiteboard::Drawing::StreamEvent>* stream) {
	Whiteboard::Drawing::StreamEvent initialEvent;
	assert(serverManager_ != nullptr);
	if (!stream->Read(&initialEvent) || !serverManager_->IsValidConnectionId(initialEvent.connectionid())) {
		return grpc::Status(grpc::StatusCode::UNAUTHENTICATED, "Invalid connection ID");
	}

	std::shared_ptr<ClientConnection> clientConnection = std::make_shared<ClientConnection>(initialEvent.connectionid(), stream);

	{
		std::lock_guard<std::mutex> lock(m_StreamWriterMutex);
		m_ActiveStreams.insert(clientConnection);
	}

	SendDrawablesToNewStream(clientConnection);

	Whiteboard::Drawing::StreamEvent event;
	while (stream->Read(&event)) {
		if (event.connectionid() != clientConnection->GetConnectionId()) {
			continue;
		}

		if (event.has_drawingevent() && event.drawingevent().has_drawable()) {
			Whiteboard::Types::Drawable drawable = event.drawingevent().drawable();
			BroadcastDrawable(drawable);
		}
		else if (event.has_eraseevent() && event.eraseevent().has_drawable()) {
			Whiteboard::Types::Drawable drawable = event.eraseevent().drawable();
			BroadcastErase(drawable);
		}
	}

	return grpc::Status::OK;
}

bool DrawingServiceImpl::CloseDrawingStream(int connectionId) {
	std::lock_guard<std::mutex> lock(m_StreamWriterMutex);
	for (std::shared_ptr<ClientConnection> connection : m_ActiveStreams) {
		if (connection->GetConnectionId() == connectionId) {
			m_ActiveStreams.erase(connection);
			return true;
		}
	}
	return false;
}

void DrawingServiceImpl::BroadcastDrawable(const Whiteboard::Types::Drawable& drawable) {
	{
		std::lock_guard<std::mutex> lock(m_ObjectWriterMutex);
		std::string objectId;
		if (drawable.has_line()) {
			objectId = drawable.line().id();
		}
		else if (drawable.has_rectangle()) {
			objectId = drawable.rectangle().id();
		}

		if (objectId.empty()) {
			return;
		}
		m_DrawnObjects.insert(std::make_pair(objectId, drawable));
	}

	Whiteboard::Drawing::StreamEvent event;
	event.set_connectionid(-1); // use -1 for now for server
	*event.mutable_drawingevent()->mutable_drawable() = drawable;

	std::lock_guard<std::mutex> lock(m_StreamWriterMutex);
	for (std::shared_ptr<ClientConnection> connection : m_ActiveStreams) {
		connection->SendEvent(event);
	}
}

void DrawingServiceImpl::BroadcastErase(const Whiteboard::Types::Drawable& drawable) {
	{
		std::lock_guard<std::mutex> lock(m_ObjectWriterMutex);
		std::string objectId;
		if (drawable.has_line()) {
			objectId = drawable.line().id();
		}
		else if (drawable.has_rectangle()) {
			objectId = drawable.rectangle().id();
		}

		if (objectId.empty()) {
			return;
		}
		m_DrawnObjects.erase(objectId);
	}

	Whiteboard::Drawing::StreamEvent event;
	event.set_connectionid(-1); // use -1 for now for server
	*event.mutable_eraseevent()->mutable_drawable() = drawable;

	std::lock_guard<std::mutex> lock(m_StreamWriterMutex);
	for (std::shared_ptr<ClientConnection> connection : m_ActiveStreams) {
		connection->SendEvent(event);
	}
}

void DrawingServiceImpl::SendDrawablesToNewStream(std::shared_ptr<ClientConnection> clientStream) {
	std::lock_guard<std::mutex> lock(m_ObjectWriterMutex);
	for (const std::pair<std::string, Whiteboard::Types::Drawable> pair : m_DrawnObjects) {
		Whiteboard::Drawing::StreamEvent event;
		*event.mutable_drawingevent()->mutable_drawable()  = pair.second;
		event.set_connectionid(-1);
		clientStream->SendEvent(event);
	}
}
