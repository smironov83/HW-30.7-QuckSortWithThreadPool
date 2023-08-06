#include "thread_pool.h"

ThreadPool::ThreadPool(): mThreadCount_(std::thread::hardware_concurrency() 
	!= 0 ? std::thread::hardware_concurrency() : 4), 
	mThreadQueues_(mThreadCount_) {}

ThreadPool::~ThreadPool() {}

void ThreadPool::Start()
{
	//i - индекс локальной очереди
	for (auto i = 0; i < mThreadCount_; ++i) 
		mThreads_.emplace_back(&ThreadPool::Idle, this, i);
}

void ThreadPool::Stop()
{
	for (auto i = 0; i < mThreadCount_; ++i)
	{
		//Кладет задачу-пустышку в каждую очередь для завершения потока.
		task_type emptyTask;
		mThreadQueues_[i].Push(emptyTask);
	}
	for (auto &th : mThreads_)
		if (th.joinable())
			th.join();
}

void ThreadPool::PushTask(func_type f, std::shared_ptr<int[]> arr, long l, 
	long r)
{
	//Вычисляет индекс очереди куда положить задачу.
	size_t idQueueToPush = mIndex_++ % mThreadCount_;
	//Формирует функтор.
	task_type task = [=] {f(arr, l, r); };
	//Помещает в очередь.
	mThreadQueues_[idQueueToPush].Push(task);
}

void ThreadPool::Idle(size_t qIndex)
{
	while (true) 
	{
		//Обрабатывает очередную задачу.
		task_type task_to_do;
		bool isGotTask = false;
		size_t i = 0;
		//Пытается быстро забрать задачу из любой очереди, начиная со своей.
		for (; i < mThreadCount_; ++i) 
			if (isGotTask = mThreadQueues_[(qIndex + i) % 
				mThreadCount_].FastPop(task_to_do))
				break;
		//Если нет задач. Ждёт задачу.
		if (!isGotTask)
			mThreadQueues_[qIndex].Pop(task_to_do);
		//Чтобы не допустить зависания потока кладет обратно задачу-пустышку.
		else if (!task_to_do)
			mThreadQueues_[(qIndex + i) % mThreadCount_].Push(task_to_do);
		if (!task_to_do) 
			return;
		//Выполняет задачу.
		task_to_do();
	}
}