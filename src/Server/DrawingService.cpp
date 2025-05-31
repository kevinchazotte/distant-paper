#include "DrawingService.h"

grpc::Status DrawingServiceImpl::DrawingStream(grpc::ServerContext* context,
	grpc::ServerReaderWriter<Whiteboard::Drawing::StreamEvent, Whiteboard::Drawing::StreamEvent>* stream) {
	ClientStream clientStream;
	clientStream.stream = stream;
}
