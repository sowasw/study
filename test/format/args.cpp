#include <iostream>
#include <stdio.h>
#include <stdarg.h>

//可变参数模板
//可变参数模板是递归的，定义一个给最后一个参数用的模板
template <typename T>
T& func( T& t) //给最后一个参数使用,递归终点
{
    return t;
}

template <typename T, typename... Arg>
T& func( T& t,  Arg&... a)
{
    t += func(a...);
    return t ;
}

int main()
{
    int n =1, m=2;
    std::cout << func(m,n)<<"\n";
    
    std::string str1 ="s", str2 = "w";
    std::cout << func(str1,str2)<<"\n";

    return 0;
}


