#include "thread_pool.h"

ThreadPool::ThreadPool():
	nThreads_(std::thread::hardware_concurrency() != 0 ? std::thread::hardware_concurrency() : 1),
	threadQueues_(nThreads_){}

void ThreadPool::start()
{
	for (size_t i = 0; i < nThreads_; ++i) 
		//i - ������ ��������� �������
		threads_.emplace_back(&ThreadPool::idle, this, i);
}


void ThreadPool::stop()
{
	for (size_t i = 0; i < nThreads_; i++) {
		// ������ ������-�������� � ������ �������
		// ��� ���������� ������
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
	//��������� ������ ������� ���� �������� ������
	size_t idQueueToPush = index_++ % nThreads_;
	//������������ �������
	task_type task = [=] {f(arr, l, r); };
	//��������� � �������
	threadQueues_[idQueueToPush].push(task);
}

void ThreadPool::idle(size_t qindex)
{
	while (true) {
		// ��������� ��������� ������
		task_type task_to_do;
		bool isGotTask = false;
		size_t i = 0;
		for (; i < nThreads_; ++i) 
		{
			//������� ������ ������� ������ �� ����� �������, ������� �� �����
			isGotTask = threadQueues_[(qindex + i) % nThreads_].fast_pop(task_to_do);
			if (isGotTask) 
			{
				break;
			}
		}
		//��� �����
		if (!isGotTask) 
		{
			//��� ������
			threadQueues_[qindex].pop(task_to_do);
		}
		else if (!task_to_do) 
		{
			// ����� �� ��������� ��������� ������
			// ������ ������� ������-��������
			threadQueues_[(qindex + i) % nThreads_].push(task_to_do);
		}
		if (!task_to_do) 
		{
			return;
		}
		//��������� ������
		task_to_do();
	}
}