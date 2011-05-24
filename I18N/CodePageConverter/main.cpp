#include <stdio.h>
//#include "CodePageConverter.h"
#include "cpconverter.h"

#define BUFFER_SIZE 256

void test0(ConvertWay pfn)
{
	CHAR lpSrc[BUFFER_SIZE] = "埃侣郎祇ネㄇゼ岿粇. 瞒秨ぃ纗?";
	int cbMultiByteSrc = BUFFER_SIZE;
	
	CHAR lpDst[BUFFER_SIZE] = {0};
	int cbMultiByteDst = BUFFER_SIZE;
	
	pfn(lpSrc, cbMultiByteSrc, lpDst, cbMultiByteDst);
	printf("%s --> %s\n", lpSrc, lpDst);	
}

void test1()
{
	CHAR lpSrc[BUFFER_SIZE] = "い地チ㎝瓣xy2瓣0地2345";
	int cbMultiByteSrc = BUFFER_SIZE;
	
	CHAR lpDst[BUFFER_SIZE] = {0};
	int cbMultiByteDst = BUFFER_SIZE;
	
	Big5ToGBK(lpSrc, cbMultiByteSrc, lpDst, cbMultiByteDst);
	printf("%s --> %s\n", lpSrc, lpDst);
	
	GBKToBig5(lpDst, cbMultiByteDst, lpSrc, cbMultiByteSrc);
	printf("%s --> %s\n", lpDst, lpSrc);
}

#define BUFFER_SIZE2 30

void test2()
{
	CHAR lpSrc[BUFFER_SIZE2] = "中華人民共和國xy2國0華2345";
	int cbMultiByteSrc = BUFFER_SIZE2;
	
	CHAR lpDst[BUFFER_SIZE2] = {0};
	int cbMultiByteDst = BUFFER_SIZE2;
	
	GBKToGB2312(lpSrc, cbMultiByteSrc, lpDst, cbMultiByteDst);
	printf("%s --> %s\n", lpSrc, lpDst);

	GB2312ToGBK(lpDst, -1, lpSrc, cbMultiByteSrc);
	printf("%s --> %s\n", lpDst, lpSrc);
}

void test3()
{
	CHAR lpSrc[BUFFER_SIZE2] = "い地チ㎝瓣xy2瓣0地2345";
	int cbMultiByteSrc = BUFFER_SIZE2;
	
	CHAR lpDst[BUFFER_SIZE2] = {0};
	int cbMultiByteDst = BUFFER_SIZE2;
	
	Big5ToGB2312(lpSrc, cbMultiByteSrc, lpDst, cbMultiByteDst);
	printf("%s --> %s\n", lpSrc, lpDst);

	GB2312ToBig5(lpDst, cbMultiByteDst, lpSrc, cbMultiByteSrc);
	printf("%s --> %s\n", lpDst, lpSrc);
}

int main(int argc, char* argv[])
{
	test0(&Big5ToGBK);
	test1();
	test2();
	test3();
	return 0;
}