// -------------------------------------------
// Compile options needed: /GX /MDd
//                     or: /GX /MD
// EXE.CPP

#include <iostream>
#include "MyHeader.h"

int main ()
{
	MyClass x;
	
	for (int i=0; i<5; i++) x.VectorOfInts.push_back(i);
	for (char j=0; j<5; j++) x.StaticVectorOfChars.push_back('a' + j);
	
	std::vector<int>::iterator vii = x.VectorOfInts.begin();
	while (vii != x.VectorOfInts.end())
	{
		std::cout << *vii;
		std::cout << " displayed from x.VectorOfInts" << std::endl;
		vii++;
	}
	std::vector<char>::iterator vci = x.StaticVectorOfChars.begin();
	while (vci != x.StaticVectorOfChars.end())
	{
		std::cout << *vci;
		std::cout << " displayed from MyClass::StaticVectorOfChars";
		std::cout << std::endl;
		vci++;
	}
	
	std::vector<MyClass> vy;
	for (i=0; i<5; i++) vy.push_back(MyClass());
	
	return 1;
}