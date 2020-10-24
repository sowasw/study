//年 月 日

#ifndef calendar_h
#define calendar_h

//#include<string>
#include <iostream>

enum Month 
{
	January,
	February,
	March,
	April,
	May,
	June,
	July,
	August,
	September,
	October,
	November,
	December
};

int daysInMonth(Month month, int year);//计算每月多少天

bool isLeapYear(int year);//判断闰年

std::string monthToString(Month month);//字符串输出月份


#endif
