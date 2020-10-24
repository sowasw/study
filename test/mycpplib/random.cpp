#include "random.h"
#include<cstdlib>
#include<cmath>
#include<ctime>
//#include "randompatch.h" //??stanford


//void initRandomSeed();//??//斯坦福教材此函数在电脑上没有书上说的效果//应放在接口内

int randomInt(int low, int high)
{
	double d = rand() / (double(RAND_MAX) + 1);//p69 stanford
	double s = d * (double(high) - low + 1);
	return int(floor(low+s));
}

double randowReal(double low, double high)
{
	double d = rand() / (double(RAND_MAX) + 1);//p69 stanford
	double s = d * ( high - low );
	return low + s;
}

bool randomChance(double p)
{
	return randowReal(0, 1) < p;
}

void setRandomSeed(int seed)
{
	initRandomSeed();
	srand(seed);
}

void initRandomSeed()
{
	static bool intialized = false;
	if (!intialized)
	{
		srand(int(time(NULL)));
		intialized = true;
	}
}

