#include "thread_pool.h"

ThreadPool::ThreadPool():
	nThreads_(std::thread::hardware_concurrency() != 0 ? std::thread::hardware_concurrency() : 1),
	threadQueues_(nThreads_){}

void ThreadPool::start()
{
	for (size_t i = 0; i < nThreads_; ++i) 
		//i - индекс локальной очереди
		threads_.emplace_back(&ThreadPool::idle, this, i);
}


void ThreadPool::stop()
{
	for (size_t i = 0; i < nThreads_; i++) {
		// кладем задачу-пустышку в каждую очередь
		// дл€ завершени€ потока
		task_type empty_task;
		threadQueues_[i].push(empty_task);
	}
	for (auto &t : threads_) {
		if (t.joinable()) {
			t.join();
		}
	}
}

void ThreadPool::pushTask(FuncType f, int *arr, long l, long r)
{
	//¬ычислить индекс очереди куда положить задачу
	size_t idQueueToPush = index_++ % nThreads_;
	//—формировать функтор
	task_type task = [=] {f(arr, l, r); };
	//ѕоместить в очередь
	threadQueues_[idQueueToPush].push(task);
}

void ThreadPool::idle(size_t qindex)
{
	while (true) {
		// обработка очередной задачи
		task_type task_to_do;
		bool isGotTask = false;
		size_t i = 0;
		for (; i < nThreads_; ++i) 
		{
			//ѕопытка быстро забрать задачу из любой очереди, начина€ со своей
			isGotTask = threadQueues_[(qindex + i) % nThreads_].fast_pop(task_to_do);
			if (isGotTask) 
			{
				break;
			}
		}
		//Ќет задач
		if (!isGotTask) 
		{
			//∆дЄм задачу
			threadQueues_[qindex].pop(task_to_do);
		}
		else if (!task_to_do) 
		{
			// чтобы не допустить зависани€ потока
			// кладем обратно задачу-пустышку
			threadQueues_[(qindex + i) % nThreads_].push(task_to_do);
		}
		if (!task_to_do) 
		{
			return;
		}
		//¬ыполнить задачу
		task_to_do();
	}
}