#include <chrono>
#include <iostream>
#include <vld.h>
#include "quick_sort.h"

long constexpr arrSize = 10000000;
size_t threadCount = 10;
auto main() -> int
{
  system("chcp 1251");
  system("cls");
  srand((unsigned int) time(NULL));
  std::shared_ptr<int[]> array1(new int[arrSize]);
  std::shared_ptr<int[]> array2(new int[arrSize]);
  std::shared_ptr<int[]> array3(new int[arrSize]);
  std::vector<std::thread> threads;
  auto start = std::chrono::high_resolution_clock::now();
  for (auto i = 0; i < threadCount; ++i)
    threads.push_back(std::thread([&array1, &array2, &array3, i]()
      { 
        for (auto j = arrSize / threadCount * i; j < arrSize / threadCount *
          (static_cast<unsigned long long>(i) + 1); ++j)
        {
          array1[j] = rand() % 500000;
          array2[j] = array1[j];
          array3[j] = array1[j];
        }
      }));
  for (auto &th : threads)
    if (th.joinable())
      th.join();
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsedTime = end - start;
  double seconds = elapsedTime.count();
  printf("Время создания массива: %f seconds\n", seconds);
  QuickSort quickSort(arrSize);
  
  // однопоточный запуск
  start = std::chrono::high_resolution_clock::now();
  quickSort.QuickSortSolo(array1, 0, arrSize - 1);
  end = std::chrono::high_resolution_clock::now();
  elapsedTime = end - start;
  seconds = elapsedTime.count();
  printf("Время однопоточной сортировки: %f seconds\n", seconds);
  
  // многопоточный запуск
  start = std::chrono::high_resolution_clock::now();
  quickSort.QuickSortAsync(array2, 0, arrSize - 1);
  end = std::chrono::high_resolution_clock::now();
  elapsedTime = end - start;
  seconds = elapsedTime.count();
  printf("Время многопоточной сортировки: %f seconds\n", seconds);
  for (auto i = 0; i < threads.size(); ++i)
    threads[i] = std::thread([&array2, i] ()
      {
        for (auto j = arrSize / threadCount * i; j < (arrSize / threadCount * 
          (static_cast<unsigned long long>(i) + 1) - 1); ++j)
          if (array2[i] > array2[static_cast<ptrdiff_t>(i) + 1])
            std::cout << "Отсортировано с ошибкой!!!" << std::endl;
      });
  for (auto &th : threads)
    if (th.joinable())
      th.join();
  
  // многопоточный запуск в пуле потоков
  start = std::chrono::high_resolution_clock::now();
  quickSort.QuickSortPool(array3, 0, arrSize - 1);
  end = std::chrono::high_resolution_clock::now();
  elapsedTime = end - start;
  seconds = elapsedTime.count();;
  printf("Время многопоточной сортировки: %f seconds\n", seconds);
  for (auto i = 0; i < threads.size(); ++i)
    threads[i] = std::thread([&array3, i] ()
      {
        for (auto j = arrSize / threadCount * i; j < (arrSize / threadCount * 
          (static_cast<unsigned long long>(i) + 1) - 1); ++j)
          if (array3[i] > array3[static_cast<ptrdiff_t>(i) + 1])
            std::cout << "Отсортировано с ошибкой!!!" << std::endl;
      });
  for (auto &th : threads)
    if (th.joinable())
      th.join();
  //delete[] array1;
  //delete[] array2;
  //delete[] array3;
  return 0;
}