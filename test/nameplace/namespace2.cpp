#include "namespace2.h"

long wex::l = 900;

void wex::func()
{
    wex::l = 50001;
    std::cout << "This is void wex::func. \n";
}

void wex::TestClass::print2()
{
    std::cout << "This is class wex::TestClass::print2. \n";
}