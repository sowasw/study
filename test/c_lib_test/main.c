#include <stdio.h>
#include <stdlib.h>
#include "mylib.c"

//几个库函数的实现的测试

struct a{
	int n;
	short s;
	char c;
	double d;
	long l;
};

struct a* print_a(struct a* bs);//

int main(int argc, char** argv){
	char src[14] = "hello, world!";
	char dst[20];
	char str[15];
	//char* str1;
	printf("src: %s\n", src);
	printf("strcpy_:\n%s\n", src);
	strcpy_(dst, src);
	printf("%s\n", dst);
	strcpy_(str, src);
	printf("%s\n", str);
	
	
	char dst2[100] = "wo ai ni zhonghua!";
	char str2[12] = "wo ai ni";
	strcpy_2(dst2, src);
	printf("strcpy_2:\n%s\n", dst2);
	strcpy_2(str2, src);
	printf("%s\n", str2);
	
	str[7] = '\0';
	strcpy_2(dst, src);
	printf("%s\n", dst);
	strcpy_2(str, src);
	printf("%s\n", str);
	char str3[7] = "66666";
	strcpy_2(str3, src);
	printf("%s\n", str3);
	str3[2] = '\0';
	printf("%s\n", str3);
	strcpy_2(str3, src);
	printf("%s\n", str3);
	printf("src: %s\n", src);
	
	
	char intbuf[100] = "561  -0.2  +36   7.34   aax4";
	char intbuf1[100] = "03265";
	//printf("atoi_(argv[1]): %d\n", atoi_(argv[1]));
	printf("atoi_(buf): %d\n", atoi_(intbuf1));
	printf("intbuf to atoi_: %s\n", intbuf);
	printf("atoi_(intbuf): %d %d %d %d %d\n", atoi_(intbuf + 0), 
											atoi_(intbuf + 5), 
											atoi_(intbuf + 10), 
											atoi_(intbuf + 15), 
											atoi_(intbuf + 23));
	
	memcpy_(intbuf, src, 14);
	printf("memcpy_(intbuf, src, 14): %s\n", intbuf);
	
	struct a as;
	as.n = 1;
	as.s = 2;
	as.c = 'a';
	as.d = 5.02;
	as.l = 465646546;
	struct a bs;
	memcpy_(&bs, &as, sizeof(struct a));
	printf("memcpy_(&bs, &as, sizeof(struct a)): %d %d %c %f %ld \n",
			bs.n, bs.s, bs.c, bs.d, bs.l);
	struct a cs;
	memmove_(&cs, &bs, sizeof(struct a));
	printf("memmove_(&cs, &bs, sizeof(struct a)): %d %d %c %f %ld \n",
			cs.n, cs.s, cs.c, cs.d, cs.l);
	
	bs.n = 10;
	bs.s = 20;
	bs.c = 'b';
	bs.d = 163.230;
	bs.l = 98564325;	
	char buf[1024] = {0};
	printf("sizeof(struct a): %ld\n", sizeof(struct a));
	int asize = sizeof(struct a);//24
	//0 |+++++++-+++++++-+++++++-|+++++++-+++++++-+++++++-|
	//  buf						 buf+24					  buf+48
	memmove_(buf, &as, sizeof(struct a));
	memmove_(buf + sizeof(struct a), &bs, sizeof(struct a));
	printf("buf, buf + sizeof(struct a):\n");
	print_a((struct a*)buf);
	print_a((struct a*)(buf + asize));
	
	//1 |+++++++-+++++++-|+++++++-+++++++-+++++++-|+++++++-
	//  buf				 buf+16					  buf+40
	memmove_(buf, &as, sizeof(struct a));
	memmove_(buf + 16, &bs, sizeof(struct a));//buf16-39
	printf("buf, buf + 16:\n");
	print_a((struct a*)buf);
	print_a((struct a*)(buf + 16));
	
	memmove_(buf, buf + 16, sizeof(struct a));
	printf("memmove_(buf, buf + 16, sizeof(struct a)):\n");
	print_a((struct a*)buf);
	print_a((struct a*)(buf + 16));
	
	//1 |+++++++-+++++++-+++++++-|+++++++-+++++++-|+++++++-
	//  buf				 buf+16					  buf+40
	memmove_(buf + 16, &bs, sizeof(struct a));
	memmove_(buf, &as, sizeof(struct a));//buf0-15
	printf("buf, buf + 16:\n");
	print_a((struct a*)buf);
	print_a((struct a*)(buf + 16));
	
	memmove_(buf + 16, buf, sizeof(struct a));
	printf("memmove_(buf + 16, buf, sizeof(struct a)):\n");
	print_a((struct a*)buf);
	print_a((struct a*)(buf + 16));
	
	
	
}

struct a* print_a(struct a* bs){

	printf("struct a: %d %d %c %f %ld \n",
			bs->n, bs->s, bs->c, bs->d, bs->l);
	return bs;
}

