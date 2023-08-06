#pragma once

#include <iostream>
#include <queue>
#include <future>
#include <condition_variable>
#include <vector>
#include <thread>
#include "block_queue.h"

using task_type = std::function<void()>;
//Указатель на функцию, которая является эталоном для функций задач
typedef void (*FuncType) (int *, long, long);

//Пул потоков
class ThreadPool 
{
  // количество потоков
  int mThreadCount_ = 0;
  // потоки
  std::vector<std::thread> mThreads_;
  // очереди задач для потоков
  std::vector<BlockedQueue<task_type>> mThreadQueues_;
  // для равномерного распределения задач
  int mIndex_ = 0;
public:
	ThreadPool();
	//Запуск
	void start();
	//Остановка
	void stop();
	//Добавить задачу в очередь задач
	void pushTask(FuncType f, int *arr, long l, long r);
	//Функция которую выполняют потоки если нет задач
	void idle(size_t qindex);
private:
	size_t nThreads_ = 0; //Количество потоков
	std::vector<std::thread> threads_; //Потоки
	std::vector<BlockedQueue<task_type>> threadQueues_; //Вектор локальных очередей задач потоков
	size_t index_ = 0; //Для равномерного распределения задач
};