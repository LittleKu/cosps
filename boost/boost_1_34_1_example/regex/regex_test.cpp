#include "regex_test.h"
#include <boost/regex.hpp>
#include <iostream>
#include <sstream>
#include <string>

/*
static std::string input = "Subject: Test Hello World!\n"
"Subject: Re: Test Hello World!\n"
".....\n";

static std::string szPattern = "^Subject: (Re: |Aw: )*(.*)";
*/

//static std::string input = "  Duration: 00:00:34.94, start: 0.500000, bitrate: 1272 kb/s";
static std::string input = "00:00:34.96";

//static std::string szPattern = "([\\s]*)Duration: (.*?), start: (.*?), bitrate: (.*?)";
//static std::string szPattern = "frame=(.*?) fps=(.*?) q=(.*?) (.*?)size=(.*?) time=(.*?) bitrate=(.*?)";
//static std::string szPattern = "frame=(.*?) fps=(.*?) time=(.*?) (.*?)";
static std::string szPattern = "(.*?)(\\d{2}):(\\d{2}):(\\d{2}).(\\d{2})(.*?)";

int test_regex()
{
    std::string line;
	std::istringstream iss (input, std::istringstream::in);

	try
	{
		boost::regex pat( szPattern );
		
		while (std::getline(iss, line))
		{
			boost::smatch matches;
			if (boost::regex_match(line, matches, pat))
			{
				for(int i = 0; i < matches.size(); i++)
				{
					std::cout<<i<<":["<<matches[i]<<"]"<<std::endl;
				}
				std::cout<<std::endl;
			}
		}
	}
	catch(const boost::regex_error& r)
	{
		std::cout<<"regex_error: "<<r.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"Unexpected exception of unknown type"<<std::endl;
	}
	
	return 0;
}

int test_regex2()
{
	return 0;
}