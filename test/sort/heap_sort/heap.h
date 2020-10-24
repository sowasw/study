#ifndef _heap_h_
#define _heap_h_

//typedef int key;

template<typename key>
void swap(key &m, key &n)
{
    key tmp = m;
    m = n;
    n = tmp;
}

int parentIndex(int index)
{
    //return index / 2;//算法导论的下标从1开始
    return (index - 1) / 2;//这里下标从0 开始
}

int leftIndex(int index)
{
    // return index * 2;
    return index * 2 + 1;
}

int rightIndex(int index)
{
    // return index * 2 + 1;
    return index * 2 + 2;
}

//维护大顶堆性质
template<typename key>
void maxHeapify(key *arr, int len, int index)
{
    int l = leftIndex(index);
    int r = rightIndex(index);
    int largest = index;
    if (l < len && arr[l] > arr[largest])
        largest = l;

    if (r < len && arr[r] > arr[largest])
        largest = r;

    if (largest != index)
    {
        swap(arr[index], arr[largest]);
        maxHeapify(arr, len, largest);
    }
}

//构造大顶堆
template<typename key>
void buildMaxHeap(key *arr, int len)
{
    if (len <= 1 || arr == NULL)
        return;
    for (int i = len / 2; i >= 0; i--)
        maxHeapify(arr, len, i);
}

//堆排序，升序，使用大顶堆
template<typename key>
void heapSort(key *arr, int len)
{
    if (len <= 1 || arr == NULL)
        return;
    buildMaxHeap(arr, len);//先构造最大堆

    //将堆顶元素取出至数组末尾，将末尾元素置于堆顶，
    //重新调整末尾元素之前的结构，使其满足堆定义
    //重复此过程直到排序完毕
    for (int i = len-1; i > 0; i--)
    {
        swap(arr[0], arr[i]);//堆顶元素放置在数组末尾，堆顶元素为数组中最大值

        len--;//剩下的元素从数组长度-1
        maxHeapify(arr, len, 0);//重新构造剩下的元素为大顶堆
    }
}

#endif