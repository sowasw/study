#include "random.h"
#include <cstdlib>
#include <cmath>
#include <ctime>

int randomInt(int low, int high)
{
	double d = rand() / (double(RAND_MAX) + 1);//
	double s = d * (double(high) - low + 1);
	return int(floor(low+s));
}

double randowReal(double low, double high)
{
	double d = rand() / (double(RAND_MAX) + 1);//
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

