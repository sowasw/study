#include "mysort.h"

#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<set>



//选择排序
void sort(std::vector<int> & vec)
{
	int m = vec.size();
	for (int lh = 0; lh < m; lh++)
	{
		int rh = lh;
		for (int i = lh + 1; i < m; i++)
		{
			if (vec[i] < vec[rh])
				rh = i;
		}
		int tmp = vec[lh];
		vec[lh] = vec[rh];
		vec[rh] = tmp;
	}
}

void insertSort(std::vector<int>& vec)//插入排序
{
	int n = vec.size();
	int temp;
	int lh;
	for (int rh = 0; rh < n; rh++)
	{
		temp = vec[rh];
		lh = rh;
		while (lh>0 && temp < vec[lh - 1])
		{
			vec[lh] = vec[lh - 1];
			lh--;
		}
		vec[lh] = temp;

	//	std::cout << "第" << rh + 1 << "次循环后矢量状态：";
	//	for (int i = 0; i < 8; i++)
	//		std::cout << vec[i] << " ";
	//	std::cout << std::endl;
	}
}

//归并排序
void sortguibing(std::vector<int> & vec)
{
	int n = vec.size();
	if (n <= 1)return;
	std::vector<int> v1;
	std::vector<int>v2;
	for (int i = 0; i < n; i++)
	{
		if (i < n / 2)
		{
			v1.push_back(vec[i]);
		}
		else
		{
			v2.push_back(vec[i]);
		}
	}
	sortguibing(v1);
	sortguibing(v2);
	vec.clear();
	merge(vec, v1, v2);
}

void merge(std::vector<int> &vec, std::vector<int> & v1, std::vector<int> & v2)
{
	int n1 = v1.size();
	int n2 = v2.size();
	int p1 = 0;
	int p2 = 0;
	while (p1 < n1&&p2 < n2)
	{
		if (v1[p1] < v2[p2])
		{
			vec.push_back(v1[p1++]);
		}
		else
		{
			vec.push_back(v2[p2++]);
		}
	}
	while (p1 < n1)vec.push_back(v1[p1++]);
	while (p2 < n2)vec.push_back(v2[p2++]);
}

//快速排序
void sortkuaisu(std::vector<int>& vec)
{
	quicksort(vec, 0, vec.size() - 1);
}

void quicksort(std::vector<int>& vec, int start, int finish)
{
	if (start >= finish)return;
	int boundary = partition(vec, start, finish);
	quicksort(vec, start, boundary - 1);
	quicksort(vec, boundary + 1, finish);
}

int partition(std::vector<int>& vec, int start, int finish)
{
	int pivot = vec[start];
	int lh = start + 1;
	int rh = finish;
	while (true)
	{
		while (lh < rh&&vec[rh] >= pivot)rh--;
		while (lh < rh&&vec[lh] < pivot)lh++;
		if (lh == rh)break;
		int tmp = vec[lh];
		vec[lh] = vec[rh];
		vec[rh] = tmp;
	}
	if (vec[lh] >= pivot)return start;
	vec[start] = vec[lh];
	vec[lh] = pivot;
	return lh;
}

/**********通过栈** 而不是递归实现快速排序*********/
void quickSortUseStack(int *a, int left, int right)
{
	if (a == NULL || left < 0 || right <= 0 || left>right)
		return;
	std::stack<int>temp;  //栈
	int i, j;
	//（注意保存顺序）先将初始状态的左右指针压栈
	temp.push(right);//先存右指针
	temp.push(left);//再存左指针
	while (!temp.empty())
	{
		i = temp.top();//先弹出左指针
		temp.pop();
		j = temp.top();//再弹出右指针
		temp.pop();
		if (i < j)
		{
			int k = partitionForArray(a, i, j);
			if (k > i)
			{
				temp.push(k - 1);//保存中间变量
				temp.push(i);  //保存中间变量 
			}
			if (j > k)
			{
				temp.push(j);
				temp.push(k + 1);
			}
		}

	}
}

int partitionForArray(int* a, int left, int right)
{
	int pivot = a[left];
	int i = left + 1, j = right;
	int tmp;
	
	while (true)
	{
		while (i < j && a[j] >= pivot)//
			j--;
		while (i < j && a[i] < pivot)
			i++;
		if (i == j) break;
		tmp = a[i];
		a[i] = a[j];
		a[j] = tmp;
	}
	
	if (a[i] >= pivot)return left;
	a[left] = a[i];
	a[i] = pivot;
	return i;
}
/***************************栈实现快速排序*********/


void sort(int array[], int n)//数组升序排序
{
	for (int lh = 0; lh < n; lh++)
	{
		int rh = lh;
		for (int i = lh + 1; i < n; i++)
		{
			if (array[i] < array[rh])
				rh = i;
		}
		swap(&array[lh], &array[rh]);//调用
	}
}

void swap(int * px, int * py)//swap函数用指针实现
{
	int tmp = *px;
	*px = *py;
	*py = tmp;
}

bool subsetSumExists(std::set<int>& set, int target)//子集求和
{
	//std::cout << "target1->" << target << std::endl;
	if (set.empty()) //STL
	{
		//std::cout << "empty->" << "target1->" << target << std::endl;
		return target == 0;
	}
	else
	{
		//set.begin();
		int element = *(set.begin());
		//std::set<int>rest = set.erase(element);//不能赋值？  

		set.erase(set.begin()); //先删除，此时set变小了
		std::set<int>rest;
		rest = set;             //把删掉元素的set拷贝给rest
		set.insert(element);    //再插入删掉的元素，set还原  
								//std::cout << "element->" << element << "    target2->" << target << std::endl;
		return subsetSumExists(rest, target)
			|| subsetSumExists(rest, target - element);		//包含/排除模式
	}
}

void produceNext(int* nextArr, const std::string& pattern) {
	int i = -1, j = 0, end = pattern.length() - 1;
	*(nextArr) = -1;
	while (j < end) {
		if (i == -1 || pattern[j] == pattern[i]) {
			i++;
			j++;
			// consider the next character: pattern[i] ?= pattern[j]
			if (pattern[i] != pattern[j])
				*(nextArr + j) = i;
			else
				*(nextArr + j) = *(nextArr + i);
		}
		else {
			i = *(nextArr + i);
		}
	}
}

void kmp(const std::string& match, const std::string& pattern) {
	int lenP = pattern.length();
	int* nextArr = new int[lenP];

	produceNext(nextArr, pattern);

	int posP = -1, posM = -1, lenM = match.length();

	while (posP < lenP && posM < lenM) //d  lend?
	{
		if (posP == -1 || pattern[posP] == match[posM])
		{
			posP++;
			posM++;
		}
		else {
			posP = nextArr[posP];
		}
	}

	if (posP != lenP)
		printf("I cannot find a substring of MATCH!\n");//s  lens?
	else
		printf("I've found  a substring start from: %d.\n", posM - lenP);//s lens?

	delete[] nextArr;
}

int findInSortedVector(std::string key, std::vector<std::string>& vec)
{
	return binarySearch(key, vec, 0, vec.size()-1);
}

int binarySearch(std::string key, std::vector<std::string>& vec, int p1, int p2)
{
	if (p1 > p2)
		return -1;
	int mid = (p1 + p2) / 2;
	if (key == vec[mid])
		return mid;
	if (key < vec[mid]) {
		return binarySearch(key, vec, p1, mid - 1);
	}
	else {
		return binarySearch(key, vec, mid + 1, p2);
	}
}
