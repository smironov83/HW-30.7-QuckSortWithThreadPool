#pragma once
#include "thread_pool.h"

/*Обработчик задач - закидывает задачи на исполнение
в свой внутренний пул потоков*/
class RequestHandler 
{
	//Пул потоков.
	ThreadPool threadPool_;
	public:
		RequestHandler();
		~RequestHandler();
		//Помещает задачу на выполнение.
		void PushRequest(func_type f, std::shared_ptr<int[]> arr, long l, long r);
};