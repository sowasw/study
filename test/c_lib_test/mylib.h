#ifndef _MYLIB_H_
#define _MYLIB_H_

#include <stdlib.h>

//几个库函数的实现
char* strcpy_(char* dst, char* src);
char* strcpy_2(char* dst, char* src);//dst较小时，遇到'\0'时停止复制

int atoi_(char* ptr);

void* memcpy_(void* dst, void* src, size_t size);
void* memmove_(void* dst, void* src, size_t size);//处理dst与src重叠的情况


#endif
