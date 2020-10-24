#ifndef namespace_h_
#define namespace_h_

#include <iostream>
#include <sstream>

namespace wex
{
class a
{
public:
    a() : i(9) {}
    a(int i) : i(i) {}

    int get() const { return i; }
    void set(const int i) { this->i = i; }

    std::string toString() const
    {
        std::ostringstream os;
        os << "wex::class a: " << i;
        return os.str();
    }

private:
    int i;
};

int n;
typedef std::string string;
} // namespace wex

//wex::n=10;

class a
{
public:
    a() : i(1) {}
    //a(int i):i(i) {}
    //a(int i){ this->i = i; }
    //explicit a(int i){ this->i = i; }//不能隐式转换，如func(20),不能用20构造此类，如果是上面两个构造函数声明就可以；effective c++

    int get() const { return i; }
    void set(const int i) { this->i = i; }

    std::string toString() const
    {
        std::ostringstream os;
        os << "class a: "<< i;
        return os.str();
    }

private:
    int i;
};

#endif