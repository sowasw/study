#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

//std::enable_if， 根据条件确定类型

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, T>::type
foo(T t) //如果 T是可以算术运算的，函数返回T类型
{
    //std::cout << t << endl;
    return t * 2;
}

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value>::type
foo1(T t) //如果 T是可以算术运算的，函数返回void类型
{
    std::cout << "in foo1 :" << t + 2 << endl;
    return;
}

///尾置返回类型，与auto一起
// 同foo函数
template <typename T>
auto foo2(T t) -> typename std::enable_if<std::is_arithmetic<T>::value, T>::type
{
    return t + 2;
}

auto func(int n) -> int { return n + 9; }

int main()
{
    int n = 25;
    int m = foo<int>(n);

    cout << m << endl; //50
    foo1<int>(m);
    //foo1<string>(string("hello"));//err string不可以算术运算

    auto d = foo2<double>(3.5);
    cout << d << endl;            //5.5
    cout << foo2<int>(9) << endl; //11
                                  //cout << foo2<string>(string("hello")) << endl; //err

    cout << func(20) << endl; //29

    ///lambda
    auto fun = []() { cout << "lambda \n"; };
    fun();
    auto fun1 = [](auto i) { cout << "lambda: " << i << endl; };
    fun1(1);

    vector<int> v = {5, 4, 9, 3, 5, 7, 1};
    auto print = [](auto i) { cout << i << " "; };

    for_each(v.begin(), v.end(), print);
    cout << endl;

    //升序排序
    sort(v.begin(), v.end(), [](auto &i, auto &j) { return i < j; });
    for_each(v.begin(), v.end(), print);
    cout << endl;

    //降序
    sort(v.begin(), v.end(), [](auto &i, auto &j) { return i > j; });
    for_each(v.begin(), v.end(), print);
    cout << endl;

    sort(v.begin(), v.end(), less<int>());
    for_each(v.begin(), v.end(), print);
    cout << endl;

    [&v]() { cout << (v.empty() ? "empty\n" : "not empty\n"); }(); //生成临时lambda 并直接调用

    ///decay类型退化, is_same判断类型是否一致
    //decay<int &>::type 退化int&为int型
    cout << is_same<int &, int>::value << endl;              //false
    cout << is_same<decay<int &>::type, int>::value << endl; //true

    return 0;
}