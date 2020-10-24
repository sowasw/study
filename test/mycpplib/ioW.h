//格式化输入输出
//第四章

#ifndef ioW_h
#define ioW_h

#include<string>
#include<vector>


void removeComments(std::istream &is, std::ostream &os);//4-8删掉注释：//后  及 /*  */内内容

double stringToReal(std::string str);//4-11 字符串转为浮点数
std::string realToString(double value);//浮点数转字符串
int stringToInt(std::string str);//字符串转为整数
std::string intToString(int value);//浮点数转字符串

int getInteger(std::string prompt);//4-12从键盘输入整行读取整数   检测正确性，错误可以重新输入
double getReal(std::string prompt);//4-12从键盘输入整行读取浮点数   检测正确性，错误可以重新输入
std::string getLine(std::string prompt); //4-12从键盘输入整行读取返回字符串

void errorInIO(std::string msg);

#endif
