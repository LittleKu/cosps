#include <iostream>
#include <locale>
#include "test.h"

//http://en.cppreference.com/w/cpp/locale/locale
void test1()
{
	/*
    std::wcout << "User-preferred locale setting is " << std::locale("").name().c_str() << '\n';
    // on startup, the global locale is the "C" locale
    std::wcout << 1000.01 << '\n';
    // replace the C++ global locale as well as the C locale with the user-preferred locale
    std::locale::global(std::locale("") );
    // use the new global locale for future wide character output
    std::wcout.imbue(std::locale());
    // output the same number again
    std::wcout << 1000.01 << '\n';
	*/
	const char* strs[]= {
		"§è§Ö§ß§ß§à§ã§ä§î123",
		"§²§å§ã§ã§Ü§Ú§Û123",
		"ÖÐÎÄ123",
		NULL
	};

	std::locale def_locale;
	{
		std::cout << "Default locale setting is " << def_locale.name().c_str() << '\n';
	}
    // on startup, the global locale is the "C" locale
    for(int i = 0; strs[i] != NULL; i++)
	{
		std::cout<<strs[i]<<std::endl;
	}

    // replace the C++ global locale as well as the C locale with the user-preferred locale
    //std::locale::global(std::locale(".949") );
	std::locale::global(std::locale("polish") );
	//std::locale::global(std::locale("") );
	{
		std::locale temp_locale;
		std::cout << "New locale setting is " << temp_locale.name().c_str() << '\n';
		for(int i = 0; strs[i] != NULL; i++)
		{
			std::cout<<strs[i]<<std::endl;
		}
	}

	std::locale::global(def_locale );
	{
		std::locale temp_locale;
		std::cout << "Restored locale setting is " << temp_locale.name().c_str() << '\n';
		for(int i = 0; strs[i] != NULL; i++)
		{
			std::cout<<strs[i]<<std::endl;
		}
	}
}

int main()
{
	test1();
	main1();
	return 0;
}