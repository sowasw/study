//常用算法
#ifndef AlgorithmsW_h
#define AlgorithmsW_h

#include<string>
#include<vector>
#include<set>

//二分查找法-vector
int findInSortedVector(std::string key, std::vector<std::string>&vec);
int binarySearch(std::string key, std::vector<std::string>&vec, int p1, int p2);

void sort(std::vector<int> & vec);//选择排序

void insertSort(std::vector<int> &vec);//10-2插入排序

void sortguibing(std::vector<int> & vec);//归并排序
void merge(std::vector<int> &vec, std::vector<int> & v1, std::vector<int> & v2);

void sortkuaisu(std::vector<int> & vec);//快速排序_递归
void quicksort(std::vector<int>&vec, int start, int finish);
int partition(std::vector<int>&vec, int start, int finish);

void QuickSort(int *a, int left, int right);//栈实现快速排序，https://www.cnblogs.com/ljy2013/p/4003412.html
int Pritation(int *a, int left, int right);

//快速排序 三数取中为基准/插入快排混合策略  见testpart10

void swap(int *px, int *py);//交换
void sort(int array[], int n);//数组升序排序//选择排序

bool subsetSumExists(std::set<int>& set, int target);//子集求和

//KMP算法找子串
void produceNext(int* nextArr, const std::string& pattern);
void kmp(const std::string& match, const std::string& pattern);



#endif
