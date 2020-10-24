#include <memory>
#include <iostream>
#include <functional>
#include <pthread.h>

///1
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



///2
static void print(int n){
    std::cout<<n<<std::endl;
}
void print2(int n){
    std::cout<<n+2<<std::endl;
}

class Foo
{
public:
    typedef std::function<void()> callback;
    Foo(callback cb) : cb_(cb), n(99) {}
    Foo(callback cb, int a) : cb_(cb), n(a) {}
    
    virtual void run() { cb_(); }
    void start() { pthread_create(&tid, NULL, thread_main, this); }
    static void *thread_main(void *arg)
    {
        //Foo *f = (Foo *)arg;//ok
        Foo *f = static_cast<Foo *>(arg);//ok
        //Foo *f = static_cast<Foo *>(this); //error
        
        //run();error
        f->run();
        
        ///3
        //静态成员函数不能直接调用非静态成员变量
        //print(n);//error
        //print2(n);//error
        //print(f->n);//ok
        //print2(f->n);//ok
        //print(d);//ok，d为静态成员
        ///规则写在ThreadClass
    }
    void join() { pthread_join(tid, NULL); }
    
    ///3
    //验证全局函数及静态全局函数是否能直接调用非静态成员变量
    void doit() { print(n); }
    void doit2() { print2(n); }

protected:
//private:
    callback cb_;
private:
    pthread_t tid;
    int n;
    static int d;
};
int Foo::d = 25;

class Goo : public Foo
{
public:
    Goo(callback cb, int a):Foo(cb, a){}
    virtual void run() {
        doit();
        cb_();
    }
    
};

class Bar
{
public:
    void doit(int n) { std::cout << n << std::endl; }
};



int main()
{
    ///1
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
    //std::function<void()> vf2 = std::bind(Test::doit2, &t); //绑定动态成员函数，注意此类函数隐含一个参数为指向对象的指针
    //std::function<void()> vf3 = std::bind(t.doit2, &t);//error
    std::function<void()> vf3 = std::bind(&Test::doit2, &t); //正确的用法

    //bind绑定类成员函数时，第一个参数表示对象的成员函数的指针，第二个参数表示对象的地址。
    //必须显示的指定&Foo::print_sum，因为编译器不会将对象的成员函数隐式转换成函数指针，所以必须在Foo::print_sum前添加&；
    //使用对象成员函数的指针时，必须要知道该指针属于哪个对象，因此第二个参数为对象的地址 &foo；
    //https://www.jianshu.com/p/f191e88dcc80
    vf();
    vf3();

    ///2
    std::cout << "Foo::run(): " << std::endl;
    Bar b;
    Foo f(std::bind(&Bar::doit, &b, 57));
    f.run();
    f.start();
    f.join();
    
    ///3
    f.doit();
    f.doit2();
    
    Foo f2(std::bind(&Bar::doit, &b, 57), 30);
    f2.doit();
    f2.doit2();
    
    std::cout << "Goo::run(): " << std::endl;
    Goo g(std::bind(&Bar::doit, &b, 57), 5698);
    g.start();
    g.join();

    return 0;
}
