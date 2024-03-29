#include <iostream>
#include <stdlib.h>
#include "heap.h"

#define SIZE 100000               //数组长度
#define MAXSIZE_TO_PRINT 100 //数组大小不超过此值就打印到屏幕上

void initArrayRand(int *arr, int size);               //随机初始化数组
void printArray(int *arr, int size, std::string msg); //打印数组到屏幕上

void quickSortArray(int *arr, int size); //快速排序
void quickSortArrayRange(int *arr, int start, int finish);
int partitionArray(int *arr, int start, int finish);

int main()
{
    srand(time(0));
    int arr[SIZE];
    double start, finish;

    //堆排序
    std::cout << "heap sort..."
              << "\n";
    initArrayRand(arr, SIZE);
    printArray(arr, SIZE, "before heap sort: ");
    start = double(clock()) / CLOCKS_PER_SEC;
    heapSort(arr, SIZE); //
    //buildMaxHeap(arr, SIZE);
    //maxHeapify(arr, SIZE, 0);
    finish = double(clock()) / CLOCKS_PER_SEC;
    std::cout << "heap sort " << SIZE << " digits used " << finish - start << " secends.\n";
    printArray(arr, SIZE, " after heap sort: ");

    //快速排序,对比一下速度
    std::cout << "quick sort..."
              << "\n";
    initArrayRand(arr, SIZE);
    printArray(arr, SIZE, "before quick sort: ");
    start = double(clock()) / CLOCKS_PER_SEC;
    quickSortArray(arr, SIZE); //
    finish = double(clock()) / CLOCKS_PER_SEC;
    std::cout << "quick sort " << SIZE << " digits used " << finish - start << " secends.\n";
    printArray(arr, SIZE, " after quick sort: ");

    return 0;
}

void initArrayRand(int *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        *arr++ = rand() % size + 1;
    }
}
void printArray(int *arr, int size, std::string msg)
{
#if SIZE <= MAXSIZE_TO_PRINT
    std::cout << msg << "arr[" << size << "]: ";
    for (int i = 0; i < size; i++)
    {
        std::cout << *arr++ << " ";
    }
    std::cout << "\n";
#endif
}

void insertSortArray(int *arr, int size)
{
    int rhs;
    for (int i = 0; i < size; i++)
    {
        rhs = i;
        for (int j = rhs - 1; j >= 0; j--)
        {
            if (arr[rhs] < arr[j])
                swap(arr[j], arr[rhs--]);
        }
    }
}

void quickSortArray(int *arr, int size)
{
    quickSortArrayRange(arr, 0, size - 1);
}

void quickSortArrayRange(int *arr, int start, int finish)
{
    if (start >= finish)
        return;
    int partition = partitionArray(arr, start, finish);
    quickSortArrayRange(arr, start, partition - 1);
    quickSortArrayRange(arr, partition + 1, finish);
}

int partitionArray(int *arr, int start, int finish)
{
    int p = arr[start];
    int lhs = start + 1;
    int rhs = finish;

    ////error
    // while (true)
    // {
    //     while (p >= arr[lhs] && lhs < rhs)
    //         lhs++;
    //     while (p < arr[rhs] && lhs < rhs)
    //         rhs--;
    //     if (lhs == rhs)
    //         break;
    //     swap(arr[lhs], arr[rhs]);
    // }

    //基准p在左边，需要先移动右边的游标，否则不能得到正确结果
    //基准在左边，如果先移动左边的游标，可能导致左游标指向的数组值大于基准
    //如array：3 1 2 5 4，左游标移到5时停止，右游标也移到5，退出循环，函数返回0，而右边的array[1]小于array[0]；
    //先移动右游标则能正确处理
    while (true)
    {
        while (lhs < rhs && p <= arr[rhs]) ///这两个循环顺序不能反
            rhs--;
        while (lhs < rhs && p > arr[lhs]) ///
            lhs++;
        if (lhs == rhs)
            break;
        swap(arr[lhs], arr[rhs]);
    }

    if (p <= arr[lhs])
        return start;

    swap(arr[lhs], arr[start]);
    //arr[start] = arr[lhs];
    //arr[lhs] = p;

    return lhs;
}
