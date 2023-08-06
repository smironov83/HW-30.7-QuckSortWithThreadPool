#include "request_handler.h"

RequestHandler::RequestHandler()
{
	threadPool_.start();
}

RequestHandler::~RequestHandler()
{
	threadPool_.stop();
}

void RequestHandler::pushRequest(FuncType f, int *arr, long l, long r)
{
	threadPool_.pushTask(f, arr, l, r);
}