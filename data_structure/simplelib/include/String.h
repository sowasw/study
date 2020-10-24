#ifndef _STRING_H__
#define _STRING_H__

#include <iostream>
#include <stdlib.h>

class String {
public:
    String();
    String(const char* str);
    ~String();

    int length()const;
    char* toString()const;
    char* c_str()const; //返回一个C风格字符串
    String& insert(const char ch, const int n); //在下标n处插入字符
    String& remove(const int n); //删除下标n处字符
    String& remove(const int start, const int nChars); //删除start开始的nChars个字符

    char operator[](const int n) const;

    //必须对流用friend
    friend std::ostream & operator<<(std::ostream & os, const String & str); //友元函数重载<<操作符
    friend std::istream & operator>>(std::istream & is, const String & str); //友元函数重载>>操作符

    int compare(const String &lhs, const String &rhs)const; //比较
    bool operator==(const String &str)const;
    bool operator!=(const String &str)const;
    bool operator>(const String &str)const;
    bool operator<(const String &str)const;

    String& operator+=(const String &str);
    String operator+(const String &str)const;

    String& operator=(const String &str);
    String(const String &str); //复制构造函数


private:
    int len;
    char* data;


    /**迭代器
     * 使用方法:
     * String* sp = new String(s);
     * 1.
     * String::iterator it;
     * it = sp;
     * 
     * 2.
     * String::iterator it1(sp);
     * 
     * 迭代器范围：
     * (it<=end()-1);
     * (it>=begin());
     * 对于+ -运算没有检查边界
     */
public:

    class iterator {
    public:

        iterator() {
            it = NULL;
            index = 0;
        }

        iterator(const iterator &rhs) {
            it = rhs.it;
            index = rhs.index;
        }

        iterator(const String *sp) {
            it = sp;
            index = 0;
        }

        iterator(const String *sp, int n) {
            it = sp;
            index = n;
        }

        ~iterator() {
            //            delete it;//!!!!!!不能在迭代器调用delete
            it = NULL;
        }

        char operator*() {
            return *(it->data + index);
        }

        iterator operator++(int) {//注意前缀运算符的&,后缀运算符无&（返回新的迭代器，即++之前的迭代器）
            iterator copy(*this);
            operator++();
            return copy;
        }

        iterator& operator++() {
            if (it == NULL) {
                std::cout << "迭代器未初始化！程序退出！\n";
                exit(1);
            }
            index++;
            if (index > it->len)
                it = NULL;
            return *this;
        }

        iterator operator--(int) {
            iterator copy(*this);
            operator--();
            return copy;
        }

        iterator& operator--() {
            if (it == NULL) {
                std::cout << "迭代器未初始化！程序退出！\n";
                exit(1);
            }
            index--;
            if (index > it->len || index < 0)//0表示在begin,len对应end
                it = NULL;
            return *this;
        }

        bool operator==(const iterator &rhs) {
            if (it != rhs.it) {
                std::cout << "不同String迭代器！程序退出！==\n";
                exit(1);
            }
            return (it == rhs.it && index == rhs.index);
        }

        bool operator!=(const iterator &rhs) {
            if (it != rhs.it) {
                std::cout << "不同String迭代器！程序退出！!=\n";
                exit(1);
            }
            return !(*this == rhs);
        }

        bool operator<(const iterator &rhs) {
            if (it != rhs.it) {
                std::cout << "不同String迭代器！程序退出！<\n";
                exit(1);
            }
            return (index < rhs.index);
        }

        bool operator<=(const iterator &rhs) {
            if (it != rhs.it) {
                std::cout << "不同String迭代器！程序退出！<=\n";
                exit(1);
            }
            return (index <= rhs.index);
        }

        bool operator>(const iterator &rhs) {
            if (it != rhs.it) {
                std::cout << "不同String迭代器！程序退出！>\n";
                exit(1);
            }
            return (index > rhs.index);
        }

        bool operator>=(const iterator &rhs) {
            if (it != rhs.it) {
                std::cout << "不同String迭代器！程序退出！>=\n";
                exit(1);
            }
            return (index >= rhs.index);
        }

        iterator operator+(const int k) {
            if (it == NULL) {
                std::cout << "迭代器未初始化！程序退出！\n";
                exit(1);
            }
            return iterator(it, index + k);
        }

        iterator operator-(const int k) {
            if (it == NULL) {
                std::cout << "迭代器未初始化！程序退出！\n";
                exit(1);
            }
            return iterator(it, index - k);
        }

        //        friend class String;
    private:
        const String* it;
        int index;
    };

    iterator begin() const {
        return iterator(this, 0);
    }

    iterator end() const {
        return iterator(this, len);
    }
};



#endif /* STRING_H */

