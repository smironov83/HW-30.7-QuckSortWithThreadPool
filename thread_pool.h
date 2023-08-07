#pragma once
#include <future>
#include <vector>
#include "block_queue.h"

using task_type = std::function<void()>;
//��������� �� �������, ������� �������� �������� ��� ������� �����.
using func_type = void(*)(std::shared_ptr<int[]>, long, long);

//��� �������.
class ThreadPool 
{
  //���������� �������.
  size_t mThreadCount_ = 0;
	//��� ������������ ������������� �����.
	size_t mIndex_ = 0;
  //��������� �������.
  std::vector<std::thread> mThreads_;
  //������� ����� ��� �������.
  std::vector<BlockedQueue<task_type>> mThreadQueues_;
public:
	ThreadPool();
	~ThreadPool();
	//���������.
	void Start();
	//�������������.
	void Stop();
	//��������� ������ � ������� �����.
	void PushTask(func_type f, std::shared_ptr<int[]> arr, long l, long r);
	//������� ������� ��������� ������ ���� ��� �����.
	void Idle(size_t qIndex);
};