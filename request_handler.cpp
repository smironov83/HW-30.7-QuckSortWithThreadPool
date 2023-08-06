#include "request_handler.h"

RequestHandler::RequestHandler()
{
	threadPool_.Start();
}

RequestHandler::~RequestHandler()
{
	threadPool_.Stop();
}

void RequestHandler::PushRequest(func_type f, std::shared_ptr<int[]> arr, 
	long l, long r)
{
	threadPool_.PushTask(f, arr, l, r);
}