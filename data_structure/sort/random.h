//随机数

#ifndef _RANDOM_H__
#define _RANDOM_H__


int randomInt(int low, int high);//返回两个数之间随机数

double randowReal(double low, double high);//返回特定区间的随机实数

bool randomChance(double p);//以p的概率返回true

void setRandomSeed(int seed);//设置随机数种子

void initRandomSeed();//初始化随机种子为系统时间，重复用此方法不会重复初始化   


//直接设置随机种子方法
//#include<cstdlib>
//#include<ctime>
//srand(int(time(NULL)));
//srand(1);


#endif
