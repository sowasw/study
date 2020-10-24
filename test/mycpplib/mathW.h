//数学算法

#ifndef mathW_h
#define mathW_h

//#include<string>
//#include<vector>

void reversprintN();//1-8 读取整数逆序输出
bool isPrime(int n);//1-9判断素数

int roundToNearestInt(double n);//浮点转整数2-3
bool isPerfect(int n);//2-5 判断完全数
double sqrtMYsa(double n);//2-7逐步逼近法求平方根

//组合数学
int promutations(int n, int k);//2-8 排列函数，从n个元素的集合中选取有序的k个元素有多少种取法
int combinations(int n, int k);//2-9组合函数，从n个元素的集合中得到的包含k个元素的子集

bool isEven(int varx);                              //判断一个整数是否偶数
int digitSum(int varx);                             //求一个正整数各位数字之和
int gcdOJLD(int varx, int vary);                    //欧几里得求两个整数最大公约数的算法




#endif
