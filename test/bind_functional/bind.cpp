#include <memory>
#include <iostream>
#include <functional>

using namespace std;

class foo
{
public:
    foo(){}
    foo(int i){}
    void doit(){cout << "foo\n";}   
}; 

class bar
{
public:
    void doit(){cout << "bar\n";}   
};

int main()
{
    foo fo;
    //fo.doit();
    
    //function<void()> func = std::bind(&foo::doit,&foo(1));//不能从临时对象绑定
    //function<void()> func1 = std::bind(&foo::doit,new foo());//ok, 但是不能清除对象 
    function<void()> func1 = std::bind(&foo::doit,make_shared<foo>());//ok
    //function<void()> func1 = std::bind(&foo::doit,&fo);//ok
    
    func1();
    
    bar b();//err 默认构造函数不能这样用,编译器可能以为这里是函数声明而不是构造一个对象
    foo f1();//err
    foo f2(1);//ok
    //b.doit();//err
    //f1.doit();//err
    //f2.doit();//ok
    
    bar *bp = new bar();
    bp->doit();
    make_shared<bar>()->doit();
    
    return 0; 
}
