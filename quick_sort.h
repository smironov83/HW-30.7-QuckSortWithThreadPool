#pragma once
#include "request_handler.h"

struct QuickSort
{
  long threadCoef_ = 0;
  RequestHandler rh_;
  QuickSort(long arrSize) { threadCoef_ = arrSize / 1000; };
  ~QuickSort() = default;
  void QuickSortSolo(std::shared_ptr<int[]> array, long left, long right);
  void QuickSortAsync(std::shared_ptr<int[]> array, long left, long right);
  void QuickSortPool(std::shared_ptr<int[]> array, long left, long right);
};