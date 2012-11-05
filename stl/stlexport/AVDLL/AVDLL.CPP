//http://support.microsoft.com/default.aspx?scid=kb%3ben-us%3b172396
//---------------------------------------------------------
// AVDLL.CPP
// Compile options needed /GX
#pragma warning (disable : 4786)
#include <map>
#include <string>
#include <stdlib.h>

// Create the map here in the DLL
__declspec(dllexport)
std::map<int,std::string>* GiveMeAMap(int n) {
	std::map<int,std::string> *m = new std::map<int,std::string>;
	while(n--) {
		char b[33];
		itoa(n,b,2);
		(*m)[n] = std::string(b);
	}
	return m;
}

// We can access the map without error from the executable
// image where the map was created
__declspec(dllexport)
void ShowMeTheMap(std::map<int,std::string> *p) {
	int x = p->size();
	while (x--) {
		printf("%d = %s\n",x,(*p)[x].c_str());
	}
}

// An accessor method to return the associated C string
// for key x
__declspec(dllexport)
const char* MapItemX (std::map<int,std::string> *m, int x) {
	return (*m)[x].c_str();
}