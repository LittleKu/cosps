#include <stdio.h>
#include "CodePageConverter.h"

#define BUFFER_SIZE 256

void test1()
{
	CHAR lpSrc[BUFFER_SIZE] = "い地チ㎝瓣";
	int cbMultiByteSrc = BUFFER_SIZE;
	
	CHAR lpDst[BUFFER_SIZE] = {0};
	int cbMultiByteDst = BUFFER_SIZE;
	
	BIG52GBK(lpSrc, cbMultiByteSrc, lpDst, cbMultiByteDst);
	printf("%s\n", lpDst);
	
	GBK2BIG5(lpDst, cbMultiByteDst, lpSrc, cbMultiByteSrc);
	printf("%s\n", lpSrc);
}

void test2()
{
	CHAR lpSrc[BUFFER_SIZE] = "中華人民共和國";
	int cbMultiByteSrc = BUFFER_SIZE;
	
	CHAR lpDst[BUFFER_SIZE] = {0};
	int cbMultiByteDst = BUFFER_SIZE;
	
	GBK2BIG5(lpSrc, cbMultiByteSrc, lpDst, cbMultiByteDst);
	printf("%s\n", lpDst);
	
	BIG52GBK(lpDst, cbMultiByteDst, lpSrc, cbMultiByteSrc);
	printf("%s\n", lpSrc);
}

void test3()
{
	CHAR lpSrc[BUFFER_SIZE] = "中華人民共和國";
	GBK2GB(lpSrc);
	printf("%s\n", lpSrc);
}

void test4()
{
	CHAR lpSrc[BUFFER_SIZE] = "中华人民共和国";
	GB2GBK(lpSrc);
	printf("%s\n", lpSrc);
}

int main(int argc, char* argv[])
{
	test1();
	test2();
	test3();
	test4();
	return 0;
}