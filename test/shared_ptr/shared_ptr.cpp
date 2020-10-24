#include <memory>
#include <iostream>
#include <functional>

class Test
{
public:
    Test() : n(0) { std::cout << this << " Test() construct ..." << std::endl; }
    Test(int i) : n(i) { std::cout << this << " Test(int i) construct ..." << std::endl; }
    ~Test() { std::cout << this << " ~Test() ..." << std::endl; }

    int get() const { return n; }
    void set(int n) { this->n = n; }

private:
    int n;
};

int main()
{
    //std::shared_ptr<Test> tp;//指向空指针NULL
    //std::shared_ptr<Test> tp = new Test;//错误的创建对象方法
    std::shared_ptr<Test> tp(new Test);
    std::shared_ptr<Test> tp2 = std::make_shared<Test>(); //建议的创建对象方法
    std::shared_ptr<Test> tp3 = std::make_shared<Test>(9);

    std::cout << tp << std::endl;
    std::cout << tp->get() << std::endl;
    tp->set(5);
    std::cout << tp->get() << std::endl;
    std::cout << tp2->get() << std::endl;
    std::cout << tp3->get() << std::endl;

    std::cout << "tp.get() " << tp.get() << std::endl; //返回对象指针
    std::cout << "tp " << tp << std::endl;//同上

    tp = tp2; //原tp引用计数-1，这里被析构

    //delete tp.get();//错误，不能对get返回的指针调用delete
    
    std::cout << "program end here!" << std::endl;

    return 0;
}