//http://support.microsoft.com/default.aspx?scid=kb%3ben-us%3b172396
//---------------------------------------------------------
// AVEXE.CPP
// Compile options needed: /GX
#pragma warning (disable : 4786)
#include <map>
#include <string>
#include <stdio.h>

__declspec(dllimport)
std::map<int,std::string>* GiveMeAMap(int n);

__declspec(dllimport)
void ShowMeTheMap(std::map<int,std::string> *amap);

__declspec(dllexport)
const char* MapItemX (std::map<int,std::string> *m, int x);

int main () {
	
	// Create the map in the DLL
	int x = 6;
	std::map<int,std::string> *p = GiveMeAMap(x);
	
	// Display the contents of the map from the DLL
	printf("Showing contents from the DLL\n");
	ShowMeTheMap(p);
	
	// Display the contents of the map from the EXE
	// using the accessor function from the DLL so we
	// aren't directly accessing the map
	printf("Showing contents from the EXE using accessor\n");
	int i = x;
	while (i--) {
		printf("%d = %s\n",i,MapItemX(p,i));
	}
	
	// Access Violation when accessing the map that
	// was created in the DLL from the EXE
	printf("Showing contents from the EXE directly\n");
	while (x--) {
		printf("%d = %s\n",x,(*p)[x].c_str());
	}
	
	return 0;
}