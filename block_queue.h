#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>
#include <condition_variable>

//Потокобезопасная очередь
template<class T>
class BlockedQueue 
{
  std::mutex mLocker_;
  // очередь задач
  std::queue<T> mTaskQueue_;
  // уведомитель
  std::condition_variable mNotifier_;
public:
	//Поместить элемент в очередь
	void push(T &item)
	{
		std::lock_guard<std::mutex> locker(locker_);
		taskQueue_.push(item);
		//Разбудить поток, который вызвал pop чтобы забрал элемент из очереди
		notifier_.notify_one();
	}
	//Блокирующий метод получения задачи из очереди
	//Ждёт пока не появится задача
	void pop(T &item)
	{
		std::unique_lock<std::mutex> locker(locker_);
		if (taskQueue_.empty()) 
		{
			//Ждать пока какой-нибудь поток не поместит задачу в очередь (push)
			notifier_.wait(locker, [this] {return !taskQueue_.empty(); });
		}
		//Забрать задачу
		item = taskQueue_.front();
		taskQueue_.pop();
	}
	//Неблокирующий метод получения элемента из очереди
	//Если задачи нет - возвращает false, иначе - true
	bool fast_pop(T &item)
	{
		std::lock_guard<std::mutex> l(locker_);
		//Если очередь пуста - выйти
		if (taskQueue_.empty()) 
		{
			return false;
		}
		//Забрать элемент
		item = taskQueue_.front();
		taskQueue_.pop();
		return true;
	}
private:
	std::mutex locker_;
	//Очередь задач
	std::queue<T> taskQueue_;
	//Уведомитель
	std::condition_variable notifier_;
};