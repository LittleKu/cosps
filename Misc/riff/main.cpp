#include "RiffFile.h"

int main(int argc, char* argv[])
{
	const char* szFileName = "test.avi";
	if(argc >= 2)
	{
		szFileName = argv[1];
	}
	CRiffFile riff(szFileName);
	return 0;
}