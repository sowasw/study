/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "String.h"
#include <string.h>
//#include <stdlib.h>

String::String() {
    data = NULL;
    len = 0;
}

String::String(const char* str) {
    if (str == NULL) {
        data = NULL;
        len = 0;
    } else {
        len = strlen(str);
        data = new char[len + 1];
        strcpy(data, str);
        data[len] = '\0';
    }
}

String::~String() {
    if (data != NULL) {
        delete []data;
        data = NULL;
    }
}

inline int String::length()const {
    return len;
}

char* String::toString()const {
    char* str = new char[len + 1];
    if (data != NULL)
        strcpy(str, data);
    str[len] = '\0';
    return str;
}

char* String::c_str()const {
    return toString();
}

String& String::insert(const char ch, const int n) {
    int i;
    if (n < len)
        i = n;
    if (n > len - 1)
        i = len;
    if (n < 0)
        i = 0;
    len++;
    char* str = new char[len + 1];
    for (int j = 0; j < i; j++)
        str[j] = *(data + j);
    str[i] = ch;
    for (int j = i; j < len; j++)
        str[j + 1] = *(data + j);
    str[len] = '\0';
    delete []data;
    data = str;
    return *this;
}

String& String::remove(const int n) {
    return remove(n, 1);
}

String& String::remove(const int start, const int nChars) {
    if (start < 0 || nChars <= 0 || start > (len - 1))
        return *this;
    int i;
    if (start + nChars < len)
        i = nChars;
    else
        i = len - start;
    len -= i;
    for (int j = start; j < len; j++)
        *(data + j) = *(data + (j + i));
    *(data + len) = '\0';
    return *this;
}

char String::operator[](const int n) const {
    if (n > (len - 1) || n < 0) {
        std::cout << "超出边界！程序结束\n";
        exit(1);
    }
    return *(data + n);
}

std::ostream & operator<<(std::ostream & os, const String & str) {
    os << str.toString();
    return os;
}

std::istream & operator>>(std::istream & is, const String & str) {
    is >> str.toString();
    return is;
}

int String::compare(const String& lhs, const String& rhs) const {
    int n = (lhs.len < rhs.len ? lhs.len : rhs.len);
    for (int i = 0; i < n; i++) {
        if (lhs[i] > rhs[i])
            return 1;
        if (lhs[i] < rhs[i])
            return 0;
    }
    if (lhs.len > rhs.len)
        return 1;
    if (lhs.len < rhs.len)
        return 0;
    return -1; //==
}

bool String::operator==(const String &str)const {
    if (len != str.length())
        return false;
    if (len == 0)
        return true;

    int i = 0;
    while (*(data + i) == *(str.data + i)) {
        //        std::cout << i << std::endl;
        i++;
        if (i == len)
            return true;
    }
    return false;

    return (compare(*this, str) == -1); //低效
}

bool String::operator!=(const String &str)const {
    return !(*this == str);
}

bool String::operator>(const String &str)const {
    return (compare(*this, str) == 1);
}

bool String::operator<(const String &str)const {
    return (compare(*this, str) == 0);
}

String& String::operator+=(const String &str) {
    if (str.length() == 0)
        return *this;
    len = len + str.length();
    char* newstr = new char[len + 1];
    if (data != NULL) {
        strcpy(newstr, data);
        delete []data;
    }
    strcat(newstr, str.data);
    newstr[len] = '\0';
    data = newstr;
    return *this;
}

String String::operator+(const String &str)const {
    String s1;

    if (len == 0 && str.length() == 0)
        return s1;
    s1.len = len + str.length();
    s1.data = new char[s1.len + 1];
    if (data != NULL)
        strcpy(s1.data, data);
    if (str.data != NULL)
        strcat(s1.data, str.data);
    *(s1.data + s1.len) = '\0';
    return s1;

    //    s1 += *this;//低效
    //    s1 += str;
    //    return s1;
}

String& String::operator=(const String &str) {
    delete []data;

    data = new char[str.length() + 1];
    len = str.length();
    if (str.data != NULL)
        strcpy(data, str.data);
    data[len] = '\0';
    return *this;
}

String::String(const String& str) {
    data = new char[str.length() + 1];
    len = str.length();
    if (str.data != NULL)
        strcpy(data, str.data);
    data[len] = '\0';
}
