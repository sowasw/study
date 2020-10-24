#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    ifstream is;
    ofstream os;
    fstream fs;

    string str;
    string str1;

    os.open("file.txt");
    os << "hello world1\nhello world2\nhello world3\nhello world4\n";
    os.close();

    is.open("file"); //文件不存在， 不会创建新文件
    is.close();
    is.open("file.txt");

    os.open("file.os"); //文件不存在， 会创建新文件，默认会截断
    os.close();
    os.open("file.os2", std::ios_base::out);
    os << "hello world\n hello world2\n";

    if (!is.fail())
        while (getline(is, str) && !is.eof())
        {
            cout << str << endl;
            is.get();
            if (!is.eof())
                is.unget();
        }
    is.clear();//只清除流标志
    is.seekg(0, is.beg);
    cout << "after is.clear: " << endl;
    if (!is.fail())
        while (getline(is, str))
            cout << str << endl;

    //定位流
    os.seekp(100, os.beg);
    is.seekg(100, is.cur);

    //获取流位置
    os.tellp();
    is.tellg();

    os.close();
    is.close();

    //创建文件的正确方法
    ofstream os1;
    os1.open("file3");
    os1.close();

    os.open("file.os3", std::ios_base::out | std::ios_base::app); //追加
    os << "hehe\n";
    os.close();

    os.open("file.os2", std::ios_base::out); //与trunc同样的效果? 从开始位置写，行为未确定，有时会覆盖，有时会截断，最好指定trunc 或app
    os << "hello-----\n";
    os.close();

    os.open("file.os4", std::ios_base::out | ios_base::trunc);
    os << "trunc\n";
    os.close();

    fs.open("file.fs", std::ios_base::in | std::ios_base::out); //同一个fs，不能同时指定读和写;否则先使用输入流行为就成为输入流，先用输出流行为就成为输出流
    fs << "haha\nhehe\n";

    getline(fs, str);
    cout << "fstream getline: " << str << "\n";

    fs >> str1;
    cout << "fstream >>: " << str1 << "\n";

    fs.close();
    fs.open("file.fs", std::ios_base::in);
    getline(fs, str);
    cout << "fstream getline: " << str << "\n";

    fs >> str1;
    cout << "fstream >>: " << str1 << "\n";

    fs.close();
    fs.open("file.fs", std::ios_base::out); //
    fs << "heihei\n";
}