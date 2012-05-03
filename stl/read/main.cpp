#include "char_traits.h"
#include "basic_string.h"
#include "string.h"

#include <stdio.h>


int main(int argc, char* argv[])
{
	streampos spos(5);
/*	char_traits<int> intCharTraits;*/

	char_traits<int>::eof();

	string str("12345");
	string::size_type size = str.size();
	printf("%u\n", size);

	return 0;
}