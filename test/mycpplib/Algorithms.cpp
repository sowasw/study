#include<iostream>
#include<string>
#include<vector>
#include "AlgorithmsW.h"
#include "mathClass.h"
#include "random.h"

using namespace std;

int main()
{
	vector <string> vec;
	vec.push_back("a");
	vec.push_back("b");
	vec.push_back("c");
	vec.push_back("kk");
	vec.push_back("e");
	vec.push_back("f");

	cout << findInSortedVector("key", vec) << endl;
	cout << findInSortedVector("a", vec) << endl;
	cout << findInSortedVector("b", vec) << endl;
	cout << findInSortedVector("c", vec) << endl;
	cout << findInSortedVector("d", vec) << endl;
	cout << findInSortedVector("e", vec) << endl;
	cout << findInSortedVector("f", vec) << endl;

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	std::vector<int> vecint(30);
	int n = vecint.size();
	std::cout << "生成逆序序列..." << std::endl;
	for (int i = 0; i < n; i++)
	{
		vecint[i] = n - i;
		//std::cout << "第 " << i+1 << "\t" << "个元素为： " << vecint[i] << std::endl;
	}
	std::cout << "第 " << 1 << "\t" << "个元素为： " << "\t" << vecint[0] << std::endl;
	std::cout << "第 " << n << "个元素为： " << "\t" << vecint[n - 1] << std::endl;
	//std::cout << "第 " << 4222 << "\t" << "个元素为： " << "\t" << vecint[4221] << std::endl;
	//std::cout << "第 " << 4223 << "\t" << "个元素为： " << "\t" << vecint[4222] << std::endl;
	//std::cout << "第 " << 4224 << "\t" << "个元素为： " << "\t" << vecint[4223] << std::endl;

	std::cout << std::endl;
	std::cout << "快速排序..." << std::endl;
	sortkuaisu(vecint);
	std::cout << "第 " << 1 << "\t" << "个元素为： " << "\t" << vecint[0] << std::endl;
	std::cout << "第 " << n << "个元素为： " << "\t" << vecint[n - 1] << std::endl;
	//std::cout << "第 " << 4222 << "\t" << "个元素为： " << "\t" << vecint[4221] << std::endl;
	//std::cout << "第 " << 4223 << "\t" << "个元素为： " << "\t" << vecint[4222] << std::endl;
	//std::cout << "第 " << 4224 << "\t" << "个元素为： " << "\t" << vecint[4223] << std::endl;
	/*for (int i = 0; i < n; i++)
	{
		std::cout << "第 " << i+1 << "\t" << "个元素为： " << vecint[i] << std::endl;
	}*/

	Point p1;
	Point p2;
	Point p3(3, 2);
	p1.setX(3);
	p1.setY(3);

	std::cout << p1.toString() << std::endl;
	std::cout << p2.toString() << std::endl;
	std::cout << p3.toString() << std::endl;
	std::cout << (p2 == p1) << std::endl;//0
	std::cout << (p2 != p1) << std::endl;//1
	p3.set(5, 5);
	p3 = p1;
	p3 = p1+p1;
	//p3 = p3 - p1;

	std::cout << p3 << std::endl;
	//p2=p2(p3);
	//p2=p2(7, 7);
	std::cout << p2 << std::endl;

	Rational r1(3,-100);
	Rational r2(1,3);
	Rational r3(1,2);
	std::cout << r1.toString() << std::endl;
	std::cout << r2 << std::endl;
	std::cout << r3 << std::endl;
	
	std::cout << r3 + r2 << std::endl;
	std::cout << r3 - r2 << std::endl;
	std::cout << r3 * r2 << std::endl;
	std::cout << r3 / r2 << std::endl;


	int *bs;
	//*bs = { 1,2,3 };
	/*bs[0] = 1;
	bs[1] = 1;
*/
	int abs;

	char*cp;
	//cp[1] = 'n';
	//cp[2] = 's';

	char str[5] =  "hell" ;
	//str[5] = "hello";
	char *p5;
	
	char array[10] = "cnblogs";
	p5 = array;
	std::cout<<p5[0] << std::endl;
	

    return 0;
}

