#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

typedef std::shared_ptr<std::vector<std::string>> vptr;
typedef std::unique_ptr<std::vector<std::string>> vuptr;

///1
template <typename Arg>
vptr func(Arg &t) //给最后一个参数使用,递归终点
{
    vptr vp;
    vp = std::make_shared<std::vector<std::string>>();
    vp->push_back(std::to_string(t));
    return vp;
}

template <typename T, typename... Args>
vptr func(T &t, Args &... a)
{
    vptr vp;
    vp = func(a...);
    vp->push_back(std::to_string(t));
    return vp;
}

///2
template <typename Arg>
void append(vuptr &vp, Arg &&t) //给最后一个参数使用,递归终点
{
    vp->push_back(std::to_string(t));
}

template <typename T, typename... Args>
void append(vuptr &vp, T &&t, Args &&... a)
{
    vp->push_back(std::to_string(t));
    //append(vp, a...);
    append(vp, std::forward<Args>(a)...);
}

template <typename... Args>
void print(Args &&... a) //Args &&可以传递右值引用（常量）
{
    vuptr vp;
    vp = std::make_unique<std::vector<std::string>>();
    //append(vp, a...);
    append(vp, std::forward<Args>(a)...);

    std::for_each(vp->begin(), vp->end(), [](auto &i) { std::cout << i << " "; });
    std::cout << std::endl;
}

int main()
{
    int m = 1, n = 2;
    vptr vp;
    vp = func(m, n);
    std::for_each(vp->begin(), vp->end(), [](auto &i) { std::cout << i << " "; });

    std::cout << std::endl;

    print(m, n);
    print(1, 2, 3, 4, 5.5, 6.8, 7, 8, 9, 10);

    int *p = new int(12);
    std::cout << p << std::endl;
    p = nullptr;
    if (p != NULL)
        std::cout << *p << std::endl;

    std::cout << p << std::endl;
    return 0;
}
