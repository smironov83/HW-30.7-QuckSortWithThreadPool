#pragma once
#include <future>
#include <vector>
#include "block_queue.h"

using task_type = std::function<void()>;
//Указатель на функцию, которая является эталоном для функций задач.
using func_type = void(*)(std::shared_ptr<int[]>, long, long);

//Пул потоков.
class ThreadPool 
{
  //Количество потоков.
  size_t mThreadCount_ = 0;
	//Для равномерного распределения задач.
	size_t mIndex_ = 0;
  //Контейнер потоков.
  std::vector<std::thread> mThreads_;
  //Очереди задач для потоков.
  std::vector<BlockedQueue<task_type>> mThreadQueues_;
public:
	ThreadPool();
	~ThreadPool();
	//Запускает.
	void Start();
	//Останавливает.
	void Stop();
	//Добавляет задачу в очередь задач.
	void PushTask(func_type f, std::shared_ptr<int[]> arr, long l, long r);
	//Функция которую выполняют потоки если нет задач.
	void Idle(size_t qIndex);
};