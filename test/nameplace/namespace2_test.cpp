#include "namespace.h"
#include "namespace2.h"
#include "namespace2_base.h"

int main()
{

    wex::TestClass::print();//静态成员直接使用
    wex::TestClass::print("我擦嘞");//静态成员直接使用

    return 0;
}
