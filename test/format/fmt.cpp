#include <iostream>
#include <stdio.h>
#include <stdarg.h>

#define HELLO "hello%s"
//可变参数 宏
//
#define PR(...) printf(HELLO, __VA_ARGS__)
#define PRT(...) printf(__VA_ARGS__)

void print(const char *fmt, ...);             //格式化输出到标准输出
void sprint(char *buf, const char *fmt, ...); //格式化到指定缓冲区

//把上面两个函数定义为宏
#define PRINT(...) print(__VA_ARGS__)
#define SPRINT(...) sprint(buf, __VA_ARGS__)
#define SPRINT2(buffer, ...) sprint((buffer), __VA_ARGS__)//这个通用

//可变参数模板
//可变参数模板是递归的，定义一个给最后一个参数用的模板
template <typename T>
void printv(const T t) //给最后一个参数使用,递归终点
{
    std::cout << "the end:  ";
    std::cout << t << "\n";
}

template <typename T, typename... Arg>
void printv(const T t, const Arg... a)
{
    std::cout << t << ", ";
    printv(a...);
}

int main()
{
    printf("hello\n");

    PR("\n");
    PR(", world!\n");
    PR(", 马云!\n");

    PRT("hi, %s!\n", "Joe");

    char buf[500];

    print("hi!\n");
    print("hi, %s!\n", "Tim");
    print("hi, %s! %d 0x%x\n", "Tim", 2, buf);

    sprint(buf, "hi, %s! %d 0x%x\n", "Tim", 666, &buf[1]);
    print(buf);

    SPRINT("hi, %s! %d 0x%x\n", "Tim", 888, &buf[2]);
    PRINT(buf);

    SPRINT("program end!\n");
    PRINT(buf);

    SPRINT2(buf, "program end %d!\n", 2);
    PRINT(buf);

    //可变参数模板
    printv(1, 2, 3, 4);
    //printf("\n");
    printv(1, 2, "haha", 'a');

    return 0;
}

void print(const char *fmt, ...)
{
    va_list vl;

    va_start(vl, fmt); //实际是正确的，只是vs_code提示错误
    vprintf(fmt, vl);
    va_end(vl); //实际是正确的，只是vs_code提示错误
}

void sprint(char *buf, const char *fmt, ...)
{
    va_list vl;

    va_start(vl, fmt);
    vsnprintf(buf, 500, fmt, vl);

    //int n = vsnprintf(NULL, 0, fmt, vl);//返回格式化后的长度
    //printf("size of ret cstring: %d\n", n);

    va_end(vl);
}