#pragma once
#include "request_handler.h"

class QuickSort:public ThreadPool
{
public:
  long threadCoef_ = 0;
  RequestHandler rh;
  QuickSort(long arrSize) { threadCoef_ = arrSize / 1000; };
  ~QuickSort() = default;
  void QuickSortSolo(int *array, long left, long right);
  void QuickSortAsync(int *array, long left, long right);
  void QuickSortPool(int *array, long left, long right);
};