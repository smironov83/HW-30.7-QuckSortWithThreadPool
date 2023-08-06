#pragma once

#include "thread_pool.h"

//Обработчик задач - закидывает задачи на исполнение
//в свой внутренний пул потоков
class RequestHandler 
{
	public:
		RequestHandler();
		~RequestHandler();
		//Поместить задачу на выполнение
		void pushRequest(FuncType f, int * arr, long l, long r);
	private:
		// пул потоков
		ThreadPool threadPool_;
};