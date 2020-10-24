#include <iostream>
#include <string>
using namespace std;

void func(int &n) { n = 20; }
int &func1(int &n) { return n; }
int &&func2(int &n)
{
    int m = n + 1;
    //return m;
    return std::move(m);
}
int &&func3(int &&n)
{
    int &&m = n + 2;
    n = 203;
    return (int &&)m;
}

template <typename T>
void func4(T &&t) { ; }

void func5(string &&n) {}

struct foo
{
};
ostream &operator<<(ostream &os, foo &f)
{
    //ostream& os;
    os << string("foo ");
    return os;
}
typedef foo bar;
void func6(foo &&n) {}
void func7(bar &&n) {}

template <typename T>
void print_ptr(T &&t) { cout << "T&&: " << &t << endl; }

template <typename T>
void print_ptr(T &t) { cout << "T&: " << &t << endl; }

template <typename T>
void doit(T &&t)
{
    cout << "doit: " << t << " ";
    //print_ptr(t);
    print_ptr(std::forward<T>(t));
    //print_ptr(std::move(t));
}

int main()
{
    foo fo;
    bar ba;
    string str;
    int n = 0;
    int &nf = n;
    //int &nf1;
    int &&nff = 23;
    cout << nf << "\n";
    cout << nff << "\n";
    func(n);
    func4(n);
    //func5(str);//引用折叠只对模板参数有效
    //func6(fo);
    //func7(ba);
    //func3(nff);//err //nff 会被当成左值
    cout << n << "\n";

    int n2 = nf;
    int n3 = func1(n);
    int n4 = nff;
    int n5 = func2(n);
    cout << n2 << "\n";
    cout << n3 << "\n";
    cout << n4 << "\n";
    cout << n5 << "\n";
    func3((int &&)nff);
    cout << nff << "\n";
    nff = 302;
    cout << nff << "\n";
    int n6 = func3((int &&)n);
    cout << n6 << "\n";
    int n7 = func3((int &&)n);
    cout << n7 << "\n";

    int &&rff1 = std::move(n);
    cout << n << "\n";
    cout << rff1 << "\n";

    foo fo1;
    foo &fo2 = fo1;
    cout << &fo1 << endl;
    doit(fo1);               //fo1是左值，左值引用
    doit(forward<foo>(fo1)); //右值引用
    doit(fo2);               //fo2是左值，左值引用
    doit(forward<foo>(fo2));
    doit(foo());             //foo()是临时对象，右值引用
    doit("ss");              //"ss"是字符串常量，存在静态区，在c++11右值引用出现之前就是引用，左值引用
    doit("ss");              //两个"ss"地址一样
    doit(string("ws"));      //临时对象
    doit(forward<string>("es"));
    string str1("hello");
    doit(str1);
    char buf[] = "re";
    doit(buf);
    doit(forward<char *>(buf));
    doit(forward<char *>("cs"));
    int m = 9;
    doit(m);
    doit(forward<int>(m));
    doit(6); //int型常量存在栈上，相当于临时对象
    doit(forward<int>(6));
    doit(6.3);

    return 0;
}