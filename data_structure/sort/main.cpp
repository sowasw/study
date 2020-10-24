#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include "mysort.h"
#include "random.h"

using namespace std;

//#define SIZE 10   //数组长度
#define VECSIZE 30	//向量长度

void listVectorInt(std::vector<int> & vec);

void initArray(int arr[], int size);
void listArray(int arr[], int size);

int main(int argc, char** argv)
{
	if(argc != 2){
		std::cout << "请输入第二个命令行参数来设定需要排序的数组长度, 程序将调用rand将数组初始化为随机值\n";
		return 0;
	}
	int SIZE = atoi(argv[1]);//命令行参数设置需要排序的数组长度
	
	initRandomSeed();
	std::vector<int> vecint(VECSIZE);
	int n = vecint.size();
	std::cout << "create vector<int>:" << std::endl;
	for (int i = 0; i < n; i++)
	{
		//vecint[i] = n - i;
		vecint[i] = randomInt(0, VECSIZE);
		//std::cout << "第 " << i+1 << "\t" << "个元素为： " << vecint[i] << std::endl;
	}
	std::cout << "第 " << "1" << "\t" << "个元素为： " << "\t" << vecint[0] << std::endl;
	std::cout << "第 " << n << "个元素为： " << "\t" << vecint[n - 1] << std::endl;
	listVectorInt(vecint);//print

	std::cout << std::endl;
	std::cout << "快速排序..." << std::endl;
	sortkuaisu(vecint);
	std::cout << "第 " << "1" << "\t" << "个元素为： " << "\t" << vecint[0] << std::endl;
	std::cout << "第 " << n << "个元素为： " << "\t" << vecint[n - 1] << std::endl;
	listVectorInt(vecint);//print
	
	std::cout << "\n随机生成值范围为 1-"<< SIZE << "的"<< SIZE << "个元素的数组" << std::endl;
	int array[SIZE];
	initArray(array, SIZE);
	listArray(array, SIZE);
	
	quickSortUseStack(array, 0, SIZE - 1);
	listArray(array, SIZE);

    return 0;
}

void listVectorInt(std::vector<int> & vec){
	int n = vec.size();
	std::cout << "listVectorInt:" << std::endl;
	for (int i = 0; i < n; i++)
	{
		std::cout << vec[i] << " ";
	}
	std::cout << std::endl;
}

void initArray(int arr[], int size){
	for(int i=0; i<size; i++){
		arr[i] = randomInt(1, size);
	}
}
void listArray(int arr[], int size){
	int n = size;
	std::cout << "listArray:" << std::endl;
	for (int i = 0; i < n; i++)
	{
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl;
}
