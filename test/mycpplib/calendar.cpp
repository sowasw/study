#include "calendar.h"
//#include <iostream>
#include<string>

int daysInMonth(Month month, int year)
{
	switch (month)
	{
	case April:
	case June:
	case September:
	case November:
		return 30;
	case February:
		return (isLeapYear(year)) ? 29 : 28;

    /*********
	case January:
	case March:
	case May:
	case July:
	case August:
	case October:
	case December:
		return 31;
	/*********/

	default:
		return 31;
	}
}

bool isLeapYear(int year)
{
	return ((year % 4 == 0) && (year % 100 != 0))
		|| (year % 400 == 0);
}

std::string monthToString(Month month)
{
	switch (month)
	{
	case January:return "January";
	case February:return "February";
	case March:return "March";
	case April:return "April";
	case May:return "May";
	case June:return "June";
	case July:return "July";
	case August:return "August";
	case September:return "September";
	case October:return "October";
	case November:return "November";
	case December:return "December";
	default:
		return "????";
	}

	//return std::cout<<month<<" has "<< daysInMonth( month,  year)<<" days "<<std::endl;
}
