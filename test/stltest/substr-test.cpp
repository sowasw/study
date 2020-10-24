#include <iostream>
#include <string>
#include <fstream>
#include <string.h>
using namespace std;

struct strPartition
{
    strPartition(const char *partition_str) : p(partition_str) {}
    string left(string &str) { return str.substr(0, str.find(p)); }
    string right(string &str) { return str.substr(str.find(p) + strlen(p), 18446744073709551615UL); }

private:
    const char *p;
};

int main()
{
    string str;
    str = "my name is weng";
    str.substr(0, 2);
    cout << str << "+\n";
    cout << str.substr(0, 2) << "+\n"; //返回[0,0+2)之间的子串
    cout << str.substr(3, 7) << "+\n"; //返回[3,3+7)之间的子串
    auto it = str.find(" ");
    cout << str.substr(0, it) << "+\n";
    cout << str.substr(0, str.find_last_of(" ")) << "+\n";
    cout << str.substr(str.find_first_not_of(' '), str.find_last_not_of(" ")) << "+\n";
    cout << str.substr(str.find(" ") + 1, str.find_last_not_of(" ") - str.find(" ")) << "+\n";

    cout << str.substr(str.find(" ")) << "+  --8--\n";        //返回[str.find(" "),最大)之间的子串（第一个空格及之后的子串）
    cout << str.substr(str.find(" ") + strlen(" ")) << "+\n"; //返回第一个空格之后的子串

    str = string("hello world");
    cout << strPartition(" ").left(str) << "+\n";  //以" "为分割，左边的子串
    cout << strPartition(" ").right(str) << "+\n"; //以" "为分割，右边的子串
    cout << strPartition("ll").left(str) << "+\n";
    cout << strPartition("ll").right(str) << "+\n";

    //str.find()函数返回值：
    //如果找到，返回0到str.length()-1；如果没有找到返回std::string::npos, 值为 -1
    str.find(" ");
    //正确用法
    if (str.find(" ") != std::string::npos)
        ;

    //如
    std::ifstream is;
    string host;
    is.open("mysql.conf");
    while (std::getline(is, str) && !is.eof())
    {
        if (str.find("host") != std::string::npos)
        {
            host = str.substr(str.find("host") + strlen("host="), str.length());
            //str = "";
            continue;
        }
    }
}