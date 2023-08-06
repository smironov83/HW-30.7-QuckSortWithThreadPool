#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>
#include <condition_variable>

//���������������� �������
template<class T>
class BlockedQueue 
{
  std::mutex mLocker_;
  // ������� �����
  std::queue<T> mTaskQueue_;
  // �����������
  std::condition_variable mNotifier_;
public:
	//��������� ������� � �������
	void push(T &item)
	{
		std::lock_guard<std::mutex> locker(locker_);
		taskQueue_.push(item);
		//��������� �����, ������� ������ pop ����� ������ ������� �� �������
		notifier_.notify_one();
	}
	//����������� ����� ��������� ������ �� �������
	//��� ���� �� �������� ������
	void pop(T &item)
	{
		std::unique_lock<std::mutex> locker(locker_);
		if (taskQueue_.empty()) 
		{
			//����� ���� �����-������ ����� �� �������� ������ � ������� (push)
			notifier_.wait(locker, [this] {return !taskQueue_.empty(); });
		}
		//������� ������
		item = taskQueue_.front();
		taskQueue_.pop();
	}
	//������������� ����� ��������� �������� �� �������
	//���� ������ ��� - ���������� false, ����� - true
	bool fast_pop(T &item)
	{
		std::lock_guard<std::mutex> l(locker_);
		//���� ������� ����� - �����
		if (taskQueue_.empty()) 
		{
			return false;
		}
		//������� �������
		item = taskQueue_.front();
		taskQueue_.pop();
		return true;
	}
private:
	std::mutex locker_;
	//������� �����
	std::queue<T> taskQueue_;
	//�����������
	std::condition_variable notifier_;
};