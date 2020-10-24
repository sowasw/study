#include <iostream>
using namespace std;

struct foo
{
    foo() : val(0) {}
    foo(int n) : val(n) {}
    ~foo() { cout << "~foo: " << val << "\n"; }
    void set(int i) { val = i; }
    bool operator()() { cout << val << "\n"; }
    int val;
};

void func(foo f) {}
void func1(foo &f) {cout << "foo&\n";}
void func1(foo &&f) {cout << "foo&&\n";}
foo func2(foo f)
{
    foo s(99);
    return s;
}

///2
void func1(const int &f) { cout << "x&\n"; }
void func1(int &&f) { cout << "x&&\n"; }

//引用折叠原则，函数参数中的T&& 等价于T&
//引用折叠只对模板参数有效
template <typename T>
void func11(T&& t) { func1(std::forward<T>(t)); }

int main()
{
    ///1
    foo f;
    foo f1(2);
    // foo &&frf=f;
    //f1 = func2(f);
    //foo &&frf=std::move(f);
    //func1(frf);
    //func1(func2(f));
    func2(f1);
    foo f2 = func2(foo(8));
    f2.set(3);
    func(f2);

    ///2
    int n1 = 20;
    int &nlf = n1;//左值引用
    int &&nrf = n1 + 50;//右值引用，但变量名nrf会当成左值

    //这里3个变量名都通过左值引用版本调用
    func1(n1);
    func1(nlf);
    func1(nrf);

    //这里3个变量都转换为右值引用,调用右值引用版本
    func1(std::forward<int>(n1));
    func1(std::forward<int>(nlf));
    func1(std::forward<int>(nrf));

    //这里引用折叠原则，三个变量以左值引用传递如func11，forward保留左值属性， 调用左值引用版本
    func11(n1);
    func11(nlf);
    func11(nrf);
    func11(89);//常量转换为右值引用

    //3个变量先转换为右值引用再传递给函数，调用右值引用版本
    func11(std::forward<int>(n1));
    func11(std::forward<int>(nlf));
    func11(std::forward<int>(nrf));

    //先转换为右值引用再传递给函数，调用右值引用版本
    func11(std::move(n1));
    func1(std::move(n1));

    func11(f);//引用折叠原则，以左值引用传入func11， 调用左值引用版本
    func11(foo());//临时对象是右值 调用右值引用版本
    func1(std::forward<foo>(f));//先转换为右值引用再传递给函数右值引用版本
    cout << "end: ----------------------\n";
    return 0;
}