#include <memory>
#include <iostream>
#include <functional>

int func(int m, int n)
{
    return m + n;
}

class Test
{
public:
    static void doit() { std::cout << "static Test::doit" << std::endl; }
    void doit2() { std::cout << "Test::doit2" << std::endl; }
};

int main()
{
    std::cout << "here" << std::endl;

    //c++11
    //占位符std::placeholders::_1， _2
    //函数名隐式转换为函数指针
    std::function<int(int, int)> functest = std::bind(func, std::placeholders::_1,
                                                      std::placeholders::_2);
    std::function<int(int)> functest2 = std::bind(func, std::placeholders::_1, 10);
    std::function<int()> functest3 = std::bind(func, 7, 10);
    std::function<int()> functest4 = std::bind(&func, functest3(), functest2(12));

    std::cout << functest(4, 5) << std::endl;
    std::cout << functest2(4) << std::endl;
    std::cout << functest3() << std::endl;
    std::cout << functest4() << std::endl;

    std::function<void()> vf = std::bind(Test::doit); //绑定静态成员函数
    Test t;
    std::function<void()> vf2 = std::bind(&Test::doit2, &t); //绑定动态成员函数，注意此类函数隐含一个参数为指向对象的指针
    //std::function<void()> vf3 = std::bind(t.doit2, &t);//error
    std::function<void()> vf3 = std::bind(&Test::doit2, &t); //正确的用法

    //bind绑定类成员函数时，第一个参数表示对象的成员函数的指针，第二个参数表示对象的地址。
    //必须显示的指定&Foo::print_sum，因为编译器不会将对象的成员函数隐式转换成函数指针，所以必须在Foo::print_sum前添加&；
    //使用对象成员函数的指针时，必须要知道该指针属于哪个对象，因此第二个参数为对象的地址 &foo；
    //https://www.jianshu.com/p/f191e88dcc80
    vf();
    vf2();
    vf3();


    //以下为函数名与函数指针的测试
    /*
    //函数名与函数地址
    //函数名是函数类型的一个实例
    std::cout << "\n-----函数名与函数指针-----" << std::endl;
    int (*f)(int, int) = &func;
    int (*f1)(int, int) = func;

    std::cout << (void *)f << std::endl; //打印函数地址
    std::cout << func << std::endl;//1      //函数被当成bool型
    std::cout << &func << std::endl;//1
    std::cout << (void *)func << std::endl;
    std::cout << (void *)&func << std::endl; //

    std::cout << &Test::doit << std::endl;//1
    std::cout << &Test::doit2 << std::endl;//1
    std::cout << (void *)&Test::doit << std::endl;
    std::cout << (void *)&Test::doit2 << std::endl;

    std::cout << (void *)&t.doit2 << std::endl;
    std::cout << (void *)(&Test::doit2, &t) << std::endl;//返回的是对象的地址
    Test t2;
    std::cout << (void *)&t2.doit2 << std::endl;
    std::cout << (void *)(&Test::doit2, &t2) << std::endl;//返回的是对象的地址，而不是对象的成员函数地址
    std::cout << &t2 << std::endl;

    void (*f2)() = &Test::doit;
    f2();
    typedef void (*Func)();//Func为函数指针
    typedef void (Func2)();//Func2位函数类型
    f2 = (Func)&t2.doit2;
    f2();
    f2 = (Func2 *)&t2.doit2;
    f2();
    (*f2)();//调用效果一样
    void (*f3)();
    //f3 = (Func)(&Test::doit2, &t2);//会导致errdump
    f3 = (Func2 *)&t2.doit2;
    f3();
    //
    */

    return 0;
}
