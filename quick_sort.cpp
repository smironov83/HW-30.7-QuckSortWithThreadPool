﻿#include "quick_sort.h"

void QuickSort::QuickSortSolo(std::shared_ptr<int[]> array, long left, 
  long right)
{
  if (left >= right) 
    return;
  long leftBound = left;
  long rightBound = right;
  long middle = array[(static_cast<ptrdiff_t>(leftBound) + rightBound) / 2];
  do 
  {
    while (array[leftBound] < middle)
      ++leftBound;
    while (array[rightBound] > middle) 
      --rightBound;
    if (leftBound <= rightBound) 
    {
      std::swap(array[leftBound], array[rightBound]);
      ++leftBound;
      --rightBound;
    }
  } while (leftBound <= rightBound);
  QuickSortSolo(array, left, rightBound);
  QuickSortSolo(array, leftBound, right);
}

void QuickSort::QuickSortAsync(std::shared_ptr<int[]> array, long left, 
  long right)
{
  if (left >= right)
    return;
  long leftBound = left;
  long rightBound = right;
  long middle = array[(static_cast<ptrdiff_t>(leftBound) + rightBound) / 2];
  do
  {
    while (array[leftBound] < middle)
      ++leftBound;
    while (array[rightBound] > middle)
      --rightBound;
    if (leftBound <= rightBound)
    {
      std::swap(array[leftBound], array[rightBound]);
      ++leftBound;
      --rightBound;
    }
  } while (leftBound <= rightBound);

  if (rightBound - left > threadCoef_)
  {
    auto f = std::async(std::launch::async, [&] ()
      { QuickSortAsync(array, left, rightBound); });
    QuickSortAsync(array, leftBound, right);
  }
  else
  {
    QuickSortAsync(array, left, rightBound);
    QuickSortAsync(array, leftBound, right);
  }
}

void QuickSort::QuickSortPool(std::shared_ptr<int[]> array, long left, long right)
{
  if (left >= right)
    return;
  long leftBound = left;
  long rightBound = right;
  long middle = array[(static_cast<ptrdiff_t>(leftBound) + rightBound) / 2];
  do
  {
    while (array[leftBound] < middle)
      ++leftBound;
    while (array[rightBound] > middle)
      --rightBound;
    if (leftBound <= rightBound)
    {
      std::swap(array[leftBound], array[rightBound]);
      ++leftBound;
      --rightBound;
    }
  } while (leftBound <= rightBound);

  if (rightBound - left > threadCoef_*10)
  {
    std::packaged_task<void()>task([=] () 
      { rh_.PushRequest(QuickSortPool, array, left, rightBound); });
    QuickSortPool(array, leftBound, right);
    std::future<void> fut = task.get_future();
    task();
  }
  else
  {
    QuickSortPool(array, left, rightBound);
    QuickSortPool(array, leftBound, right);
  }
}

