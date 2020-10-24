#include "mathW.h"
#include <iostream>

void reversprintN()
{
	int n;
	int sum = 0;
	std::cout << "?";
	std::cin >> n;
	while (n > 0)
	{
		sum = sum * 10 + n % 10;
		n /= 10;
	}
	std::cout << "逆序输出整数：" << sum << std::endl;
}

bool isPrime(int n)
{
	if (n < 2)return false;
	else
		for (int i = 2; i < n; i++)
		{
			if (n%i == 0)return false;
		}
	return true;
}

int roundToNearestInt(double n)//浮点四舍五入转整数型的函数
{
	if (n > 0)return n + 0.5;
	if (n < 0)return n - 0.5;
	return 0;
}

bool isPerfect(int n)
{
	int sum = 0;
	for (int i = 1; i < n; i++)
	{
		if (n % i == 0)
			sum += i;
	}
	if (n == sum)return true;
	return false;
}

double sqrtMYsa(double n)
{
	double sqrt = n / 2;
	double g;
	while (true)
	{
		//i++;
		//std::cout <<"运行次数 "<< i<< std::endl;
		g = sqrt;
		sqrt = (g + n / g) / 2;//书上g/x是错误
		if (sqrt == g)
			break;
	}
	return sqrt;
}

int promutations(int n, int k)
{
	int p = 1;
	for (int i = n; i > n - k; i--)//从n乘起，乘k次
	{
		p *= i;
	}
	return p;
}

int combinations(int n, int k)
{
	int p = 1;
	for (int i = 1; i <= k; i++)//从n乘起，乘k次
	{
		p *= n--;
		p /= i;//从1除起，除k次
	}
	return p;
}

bool isEven(int varx)                       //判断一个整数的奇偶性
{
	return varx % 2 == 0;
}

int digitSum(int varx)                             //求一个正整数各位数字之和
{
	int sum = 0;
	while (varx > 0)
	{
		sum += varx % 10;                   //sum=sum+x%10   求个位数
		varx /= 10;                         //x=x/10         求其他位数，   int型舍掉小数点后面取整
	}
	return sum;
}

int gcdOJLD(int varx, int vary)                    //欧几里得求两个整数最大公约数的算法
{
	int r = varx % vary;                       //余数为0，则最大公约数为y
	while (r != 0)
	{
		varx = vary;
		vary = r;
		r = varx % vary;
	}
	return vary;
}
