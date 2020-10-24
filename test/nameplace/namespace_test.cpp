#include "namespace.h"
//#include "namespace2.h"
#include "namespace2_base.h"

void print(std::string str)
{
    std::cout << str << std::endl;
}

void dosth(a a)
{
    print(a.toString());
}

void dosth(wex::a a)
{
    print(a.toString());
}

int main()
{
    a a;
    wex::a a1;
    print(a.toString());
    print(a1.toString());
    dosth(a);
    //dosth(20);//构造函数用explicit声明,隐式转换
    dosth(30); //如果两个类都没有用explicit声明，但函数重载了参数为两个类，编译器不知道调用哪个类

    dosth(wex::l);

    wex::n = 10;
    wex::l = 100;

    dosth(wex::n);
    dosth(wex::l);

    wex::func();
    dosth(wex::l);

    wex::TestClass t;

    t.print();
    t.print2();

    wex::TestSturct ts;
    ts.c = 'c';
    ts.n = 7;

    a.set(ts.n);
    //t.print((std::string)ts.c);
    t.print(a.toString());

    wex::TestClass::print();//静态成员直接使用
    wex::TestClass::print("我擦嘞");//静态成员直接使用


    return 0;
}