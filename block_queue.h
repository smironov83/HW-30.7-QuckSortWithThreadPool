#pragma once
#include <mutex>
#include <queue>
#include <condition_variable>

//Потокобезопасная очередь.
template<class T> class BlockedQueue
{
  std::mutex mLocker_;
  //Очередь задач.
  std::queue<T> mTaskQueue_;
  //Уведомитель.
  std::condition_variable mNotifier_;
public:
	//Помещает элемент в очередь.
	void Push(T &item)
	{
		std::lock_guard<std::mutex> locker(mLocker_);
		mTaskQueue_.push(item);
		//Уведомлеяет поток, который вызвал (Pop) чтобы забрал элемент из очереди.
		notifier_.notify_one();
	}

	//Блокирующий метод получения задачи из очереди. Ожидает появление задачи.
	void Pop(T &item)
	{
		std::unique_lock<std::mutex> locker(mLocker_);
		//Ожидает пока какой-нибудь поток не поместит задачу в очередь (Push).
		if (mTaskQueue_.empty())
			notifier_.wait(locker, [this] {return !mTaskQueue_.empty(); });
		//Забирает задачу.
		item = mTaskQueue_.front();
		mTaskQueue_.pop();
	}
	/*Неблокирующий метод получения элемента из очереди. Если задачи нет - 
	возвращает false, иначе - true.*/
	auto FastPop(T &item) -> bool
	{
		std::lock_guard<std::mutex> locker(mLocker_);
		//Если очередь пуста - выходит.
		if (mTaskQueue_.empty()) 
			return false;
		//Забирает элемент.
		item = mTaskQueue_.front();
		mTaskQueue_.pop();
		return true;
	}
};