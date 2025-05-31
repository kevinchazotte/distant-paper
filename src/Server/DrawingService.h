#pragma once

#include <grpcpp/server.h>

#include "drawing_service.grpc.pb.cc"
#include "drawing_service.pb.cc"
#include "drawing_types.grpc.pb.h"
#include "drawing_types.pb.h"

class DrawingServiceImpl final : public Whiteboard::Drawing::DrawingService::Service {
public:
	grpc::Status DrawingStream(grpc::ServerContext* context, 
		grpc::ServerReaderWriter<Whiteboard::Drawing::StreamEvent, Whiteboard::Drawing::StreamEvent>* stream);

	void BroadcastDrawable(const Whiteboard::Types::Drawable& drawable);
	void BroadcastErase(const Whiteboard::Types::Drawable& drawable);

private:
	struct ClientStream {
		int client_id;
		grpc::ServerReaderWriter<Whiteboard::Drawing::StreamEvent, Whiteboard::Drawing::StreamEvent>* stream;
	};
};