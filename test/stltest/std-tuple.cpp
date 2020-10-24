#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>
using namespace std;

constexpr int expr(int i)
{
    return i;
}

int main()
{
    //auto tpl1 = {1, 2.5, "hello"};
    std::tuple<int, double, string> tpl = {1, 2.5, "hello"};

    cout << "sizeof(tuple): " << sizeof(tpl) << endl;
    std::cout << "tuple has " << std::tuple_size<decltype(tpl)>::value << " elements." << '\n';

    cout << "tuple[0]: " << get<0>(tpl) << endl;

    int i = 0;
    int n = 0;
    vector<int> v;
    v.resize(std::tuple_size<decltype(tpl)>::value);
    auto push_back = [&n](auto &e) { e = n++; }; //ok
    //auto push_back = [&](auto &e) { v[i++] = n++; };//ok

    for_each(v.begin(), v.end(), push_back);
    for_each(v.begin(), v.end(), [](auto &e) { cout << e << " "; });
    cout << endl;
    //for_each(v.begin(), v.end(), [&](auto &e) { cout << "tuple[" << e << "]: " << get<e>(tpl) << endl; });//err

    //元组无法动态遍历，只能用模板元编程 递归遍历
    

    return 0;
}