﻿#include "quick_sort.h"

void QuickSort::QuickSortSolo(int *array, long left, long right)
{
  if (left >= right) 
    return;
  long leftBound = left;
  long rightBound = right;
  long middle = array[(leftBound + rightBound) / 2];
  do 
  {
    while (array[leftBound] < middle)
      ++leftBound;
    while (array[rightBound] > middle) 
      --rightBound;
    //Меняем элементы местами
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

void QuickSort::QuickSortAsync(int *array, long left, long right)
{
  if (left >= right)
    return;
  long leftBound = left;
  long rightBound = right;
  long middle = array[(leftBound + rightBound) / 2];
  do
  {
    while (array[leftBound] < middle)
      ++leftBound;
    while (array[rightBound] > middle)
      --rightBound;
    //Меняем элементы местами
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
    // запускаем обе части синхронно
    QuickSortAsync(array, left, rightBound);
    QuickSortAsync(array, leftBound, right);
  }
}

void QuickSort::QuickSortPool(int *array, long left, long right)
{
  if (left >= right)
    return;
  long leftBound = left;
  long rightBound = right;
  long middle = array[(leftBound + rightBound) / 2];
  do
  {
    while (array[leftBound] < middle)
      ++leftBound;
    while (array[rightBound] > middle)
      --rightBound;
    //Меняем элементы местами
    if (leftBound <= rightBound)
    {
      std::swap(array[leftBound], array[rightBound]);
      ++leftBound;
      --rightBound;
    }
  } while (leftBound <= rightBound);

  if (rightBound - left > threadCoef_*10)
  {
    rh.pushRequest(QuickSortPool, array, left, rightBound);
    QuickSortPool(array, leftBound, right);
  }
  else
  {
    // запускаем обе части синхронно
    QuickSortPool(array, left, rightBound);
    QuickSortPool(array, leftBound, right);
  }
}

