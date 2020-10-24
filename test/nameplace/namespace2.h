#ifndef namespace2_h_
#define namespace2_h_

#include <iostream>
//#include "namespace2_base.h"

namespace wex
{
class TestClass;   //定义在本文件
struct TestSturct; //定义在namespace2_base.h

extern long l; //定义在namespace2.cpp

void func(); //定义在namespace2.cpp
} // namespace wex

class wex::TestClass
{
public:
    static void print()
    {
        std::cout << "This is class wex::TestClass::print(). \n";
    }

    static void print(std::string str)
    {
        std::cout << "This is class wex::TestClass::print(std::string str)):\n\t";
        std::cout << str << "\n";
    }

    void print2(); //定义在namespace2.cpp
};

#endif