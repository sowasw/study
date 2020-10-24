#include "ioW.h"

#include<iostream>
#include<string>
//#include<iomanip>
//#include<cmath>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<istream>


//#include "random.h"

void removeComments(std::istream & is, std::ostream & os)
{
	//char ch;
	std::string str = "";
	std::string str1 = "";
	int n = 0;
	while (std::getline(is, str)) {
		for (int i = 0; i < str.length(); i++) {
			if (str[i] == '/') {
				if (n == 0) {
					if (str[i + 1] == '/') {
						str = str.substr(0, i);
						break;
					}
					if (str[i + 1] == '*') {
						str1 = str.substr(0, i);
						os << str1;
						i++;// 下次索引从“/*”之后开始
						n++;
					}
				}
			}
			if (str[i] == '*') {
				if (n > 0) {
					if (str[i + 1] == '/') {
						str.erase(0, i + 2);//删除0起i位置有i+1个，再加下一个元素‘/’
						i = -1;//删除后 下次索引位置为 i+1=0
						n = 0;
					}
				}
			}
		}
		if (n == 0) {
			if (is.eof()) {
				os << str;//结尾不用加换行符
			}
			else
				os << str << "\n";//流读取行是没有读取换行符的，加上
		}
	}
}

double stringToReal(std::string str)
{
	std::istringstream iss(str);
	double value;
	iss >> value >> std::ws;
	if (iss.fail() || !iss.eof()) {
		errorInIO("无法正确输出小数");
	}
	return value;
}

std::string realToString(double value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

int stringToInt(std::string str)
{
	std::istringstream iss(str);
	int value;
	iss >> value >> std::ws;
	if (iss.fail() || !iss.eof()) {
		errorInIO("无法正确输出整数");
	}
	return value;
}

std::string intToString(int value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

int getInteger(std::string prompt)
{
	int value;
	std::string line;
	while (true) {
		if (prompt == "") prompt = "输入一个整数：";
		std::cout << prompt;
		std::getline(std::cin, line);
		std::istringstream stream(line);
		stream >> value >> std::ws;
		if (!stream.fail() && stream.eof())
			break;
		std::cout << "输入格式错误，重新输入：" << std::endl;
		//if (prompt == "") prompt = "输入一个整数：";
	}
	return value;
}

double getReal(std::string prompt)
{
	double value;
	std::string line;
	while (true) {
		if (prompt == "") prompt = "输入一个小数：";
		std::cout << prompt;
		std::getline(std::cin, line);
		std::istringstream stream(line);
		stream >> value >> std::ws;
		if (!stream.fail() && stream.eof())
			break;
		std::cout << "输入格式错误，重新输入：" << std::endl;
		//if (prompt == "") prompt = "输入一个小数：";
	}
	return value;
}

std::string getLine(std::string prompt) {
	std::string line;
	std::cout << prompt;
	std::getline(std::cin, line);
	return line;
}


void errorInIO(std::string msg)
{
	std::cerr << msg << std::endl;
	exit(EXIT_FAILURE);
}
