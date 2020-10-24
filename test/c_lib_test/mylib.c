#include "mylib.h"

char* strcpy_(char* dst, char* src){
	char* rptr = dst;
	if((src != dst) && (src != NULL) && (dst != NULL)){
		while(*dst++ = *src++);
	}
	return rptr;
}

char* strcpy_2(char* dst, char* src){
	char* rptr = dst;
	if((src != dst) && (src != NULL) && (dst != NULL)){
		while(*dst != '\0' && *src != '\0'){
			*dst++ = *src++;
		}
	}
	return rptr;
}

int atoi_(char* ptr){
	int n = 0;
	int sign = 1;
	
	while(*ptr == ' ')
		ptr++;
	
	if(*ptr == '-')
		sign = -1;
	if(*ptr == '+' || *ptr == '-')
		ptr++;
	
	while(*ptr >= '0' && *ptr <= '9'){
		n = n * 10 + *ptr - '0';
		ptr++;
	}
	return n * sign;
}

void* memcpy_(void* dst, void* src, size_t size){
	if((src == dst) || (src == NULL) || (dst == NULL))
		return dst;
	void* rptr = dst;
	char* dstptr = (char*)dst;
	char* srcptr = (char*)src;
	while(size-- > 0)
		*dstptr++ = *srcptr++;
	return rptr;
}

void* memmove_(void* dst, void* src, size_t size){
	if((src == dst) || (src == NULL) || (dst == NULL))
		return dst;
	void* rptr = dst;
	char* dstptr = (char*)dst;
	char* srcptr = (char*)src;
	if(dst < src)
		while(size-- > 0)
			*dstptr++ = *srcptr++;
	if(dst > src){
		dstptr += size - 1;
		srcptr += size - 1;
		while(size-- > 0)
			*dstptr-- = *srcptr--;
	}
	
	return rptr;
}
