#pragma once

#include <iostream>
#include <queue>
#include <future>
#include <condition_variable>
#include <vector>
#include <thread>
#include "block_queue.h"

using task_type = std::function<void()>;
//��������� �� �������, ������� �������� �������� ��� ������� �����
typedef void (*FuncType) (int *, long, long);

//��� �������
class ThreadPool 
{
  // ���������� �������
  int mThreadCount_ = 0;
  // ������
  std::vector<std::thread> mThreads_;
  // ������� ����� ��� �������
  std::vector<BlockedQueue<task_type>> mThreadQueues_;
  // ��� ������������ ������������� �����
  int mIndex_ = 0;
public:
	ThreadPool();
	//������
	void start();
	//���������
	void stop();
	//�������� ������ � ������� �����
	void pushTask(FuncType f, int *arr, long l, long r);
	//������� ������� ��������� ������ ���� ��� �����
	void idle(size_t qindex);
private:
	size_t nThreads_ = 0; //���������� �������
	std::vector<std::thread> threads_; //������
	std::vector<BlockedQueue<task_type>> threadQueues_; //������ ��������� �������� ����� �������
	size_t index_ = 0; //��� ������������ ������������� �����
};