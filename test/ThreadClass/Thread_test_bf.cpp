#include "Thread.h"
#include <iostream>
#include <unistd.h>
using namespace std;

class Foo
{
public:
    Foo(int i) : count(i)
    {
    }

    void doit()
    {
        while (count--)
            cout << "this is Foo::doit" << endl;
    }

    void doit2(int n)
    {
        count = n;
        while (count--)
            cout << "this is Foo::doit2" << endl;
    }

private:
    int count;
};

void func()
{
    cout << "this is func" << endl;
}

void func2(int n)
{
    while (n--)
        cout << "this is func2" << endl;
}

int main()
{
    Thread2 t1(func); //可以省略取地址符号&，（&func）
    Thread2 t2(std::bind(func2, 2));

    Foo foo(5);
    Thread2 t3(std::bind(&Foo::doit, &foo)); //绑定类的成员函数不能省略&,每个成员函数都隐含一个参数this指针指向当前对象，并且是其第一个参数
    Foo foo2(5);
    Thread2 t4(std::bind(&Foo::doit2, &foo2, 7));

    t1.start();
    t2.start();
    t3.start();
    t4.start();
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    
    

    return 0;
}
