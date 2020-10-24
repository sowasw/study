#include <iostream>
#include <deque>
#include <functional>
#include <algorithm>
#include <memory>
#include <map>
#include <string>

int add(int a, int b);

// for map
void printmap(std::map<std::string, int>::reference re); //refrence是取地址传参
void printmap2(std::pair<std::string, int> p);
class Printmap : public std::unary_function<std::pair<std::string, int>&, bool>//STL源码剖析，取得一元函数的参数类型和返回值类型
{
public:
   /*  bool operator()(std::pair<std::string, int> p)
    {
        std::cout << "\"" << p.first << "\" : " << p.second << " \n";
    } */
    bool operator()(std::map<std::string, int>::reference p)
    {
        std::cout << "\"" << p.first << "\" : " << p.second << " \n";
    }
};

int main()
{
    ///deque
    std::cout << "\n----deque -----\n";
    std::deque<int> di;
    di.push_back(10);
    di.push_back(20);
    di.push_front(50);
    for (int i = 0; i < di.size(); i++)
        std::cout << di[i] << " ";
    std::cout << " \n";
    std::cout << di.front() << " \n";
    std::cout << di.back() << " \n";
    di.pop_back();
    std::cout << di.back() << " \n";

    auto iter = di.begin();
    while (iter != di.end())
    {
        std::cout << *iter << " ";
        ++iter;
    }

    ///functinal
    std::cout << "\n----functional -----\n";
    std::less<int> l;
    std::cout << l(1, 3) << " \n";                              //1<3?
    std::cout << std::bind2nd(l, 0)(1) << " \n";                //1<0?
    std::cout << std::bind2nd(l, 2)(1) << " \n";                //1<2?
    std::cout << std::bind1st(l, 0)(-1) << " \n";               //0<-1?
    std::cout << std::bind1st(l, 0)(1) << " \n";                //0<1?
    std::cout << std::bind1st(std::less<int>(), 2)(7) << " \n"; //2<7?
    std::cout << std::bind2nd(std::less<int>(), 2)(7) << " \n"; //7<2?

    std::cout << std::not1(std::bind2nd(std::less<int>(), 2))(7) << " 7>=2?\n"; //
    std::cout << std::not1(std::bind2nd(std::less<int>(), 2))(2) << " 2>=2?\n"; //
    std::cout << not1(bind2nd(std::less<int>(), 2))(0) << " 0>=2?\n";           //
    //std::cout << not2(bind2nd(std::less<int>(), 2))(0) << " 0>=2?\n";           //bind2nd 返回一元对象
    std::cout << not2(std::less<int>())(0, 1) << " 0>=1?\n";   //
    std::cout << not2(std::less<int>())(0, 0) << " 0>=0?\n";   //
    std::cout << not2(std::less<int>())(0, -1) << " 0>=-1?\n"; //

    std::cout << std::ptr_fun(add)(1, 2) << " 1+2=?\n"; //ptr_fun

    ///allocator
    std::cout << "\n----allocator：allocate construct -----\n";
#define SIZE 10
    std::allocator<int> ai;
    int *ip = ai.allocate(10); //分配内存 10个整数的数组
    int *q = ip;
    ai.construct(q++); //构造第一个对象
    for (int i = 0; i < 3; i++)
        ai.construct(q++, 10 + i); //构造第2~6个对象

    int tocopy = 99;
    std::uninitialized_copy(ip, ip + 2, q); //把ip~ip+2之间的2个对象复制到由q开始的2个未初始化对象的空间中
    q += 2;
    std::uninitialized_fill(q, q + 2, tocopy); //填充后2个未初始化的对象为tocopy的内容
    q += 2;
    std::uninitialized_fill_n(q, 2, 88); //与上一句作用相同

    std::cout << " *ip = " << *ip << "\n";

    q = ip;
    for (int i = 0; i < 10; i++)
        std::cout << " *q = " << *q++ << "\n";
    //std::cout << " *q = " << *q++ << "\n";

    q = ip;
    std::cout << "\n---destroy deallocate-----\n";
    for (int i = 0; i < 10; i++)
    {
        ai.destroy(q); //destroy调用类的析构函数，对基本类型int无作用
        //ai.deallocate(q, 1); //释放内存
        //std::cout << " *q = " << *q << "\n";//释放内存后引起stackdump
        q++;
    }

    ///map
    std::cout << "\n----map pair -----\n";
    std::map<std::string, int> sim;

    //map插入数据的3种方法
    std::pair<std::string, int> value;
    value = std::pair<std::string, int>("wang", 26);
    sim.insert(value);
    sim.insert(std::map<std::string, int>::value_type("liu", 25));
    sim.insert(std::map<std::string, int>::value_type("huang", 30));
    sim["sun"] = 31;

    std::cout << sim["wang"] << " sim[\"wang\"]\n";
    std::for_each(sim.begin(), sim.end(), printmap);
    //map删除元素
    std::map<std::string, int>::iterator toerase = sim.find("liu");
    sim.erase(toerase);
    std::cout << "\nafter erase: \n";
    std::for_each(sim.begin(), sim.end(), printmap2);
    std::cout << "\nreverse for_each: \n";
    std::for_each(sim.rbegin(), sim.rend(), printmap2);
    std::cout << "\nfor_each used functionobj: \n";
    std::for_each(sim.begin(), sim.end(), Printmap()); //用仿函数遍历

    sim.insert(std::map<std::string, int>::value_type("huang", 40));//重复插入相同的键无效
    sim.insert(std::map<std::string, int>::value_type("qiao", 40));
    std::cout << "\nfor_each used function adapter: \n";
    std::for_each(sim.begin(), sim.end(), ptr_fun(printmap2)); //OK,用仿函数适配器遍历

    return 0;
}

int add(int a, int b)
{
    return a + b;
}

void printmap(std::map<std::string, int>::reference re)
{
    std::cout << "\"" << re.first << "\" : " << re.second << " \n";
}

void printmap2(std::pair<std::string, int> p)
{
    std::cout << "\"" << p.first << "\" : " << p.second << " \n";
}
