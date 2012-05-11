#include <stdio.h>
#include <string.h>
#include <iconv.h>
#include <string>

#pragma setlocale(".936")

bool bDebug = true;

int convert(const char* in_file, const char* in_encoding, const char* out_file, const char* out_encoding);
void test();

int main(int argc, char* argv[])
{
	int rc = 0;
//	rc = convert("input.txt", "UTF-8", "output.txt", "UCS-2BE");

	test();

	return rc;
}

int convert(const char* in_file, const char* in_encoding, const char* out_file, const char* out_encoding)
{
	int rc = -1;
	
	FILE* fp_in = NULL;
	FILE* fp_out = NULL;
	iconv_t cd = NULL;

	const int IN_BUF_SIZE = 4096;
	char in_buf_base[IN_BUF_SIZE];

	const int OUT_BUF_SIZE = IN_BUF_SIZE * 4;
	char out_buf_base[OUT_BUF_SIZE];

	do 
	{
		fp_in = fopen(in_file, "rb");
		if(fp_in == NULL)
		{
			printf("Failed to open in_file: %s\n", in_file);
			break;
		}

		fp_out = fopen(out_file, "wb");
		if(fp_in == NULL)
		{
			printf("Failed to open out_file: %s\n", out_file);
			break;
		}

		cd = iconv_open (out_encoding, in_encoding);

		char* inbuf = in_buf_base;
		size_t inbytesleft = IN_BUF_SIZE;

		char* outbuf = out_buf_base;
		size_t outbytesleft = OUT_BUF_SIZE;

		size_t niconv = 0;
		size_t nread = 0, nleft = 0;

		while( (nread = fread(inbuf + nleft, 1, IN_BUF_SIZE - nleft, fp_in)) > 0 )
		{
			inbytesleft = nleft + nread;
			outbytesleft = OUT_BUF_SIZE;

			if(bDebug)
			{
				printf("inbuf=0x%08X, outbuf=0x%08X\n", inbuf, outbuf);
			}
			
			niconv = iconv(cd, (const char**)&inbuf, &inbytesleft, (char**)(&outbuf), &outbytesleft);

			if(bDebug)
			{
				printf("inbuf=0x%08X, outbuf=0x%08X\n", inbuf, outbuf);
				printf("niconv=%d, inbuf_size=%d, inbytesleft=%d, outbytesleft=%d\n", niconv, nleft + nread, inbytesleft, outbytesleft);
			}

			fwrite(out_buf_base, 1, OUT_BUF_SIZE - outbytesleft, fp_out);

			nleft = inbytesleft;
			if(nleft > 0)
			{
				memmove(in_buf_base, inbuf, nleft);
			}
			inbuf = in_buf_base;
			outbuf = out_buf_base;
		}
		
		//OK
		rc = 0;

	} while (false);

	//Clean up
	if(fp_in)
	{
		fclose(fp_in);
		fp_in = NULL;
	}
	if(fp_out)
	{
		fclose(fp_out);
		fp_out = NULL;
	}
	if(cd)
	{
		iconv_close(cd);
		cd = NULL;
	}

	return rc;
}

char getByte(wchar_t ch, bool hi)
{
	if(hi)
	{
		return (char)((ch & 0xFF00) >> 8);
	}
	else
	{
		return (char)(ch & 0x00FF);
	}
}

size_t getBytes(const std::wstring& s, char* buffer)
{
	for(int i = 0, length = s.length(); i < length; i++)
	{
		wchar_t ch = s.at(i);
		buffer[i * 2] = getByte(ch, false);
		buffer[i * 2 + 1] = getByte(ch, true);
	}
	return length * 2;
}

size_t getBytes(const std::wstring& s, char* buffer, size_t buffer_size, const char* charset)
{
	const int BUF_SIZE = 128;

	char input[BUF_SIZE];
	size_t input_len = getBytes(s, input);
	
	char* inbuf = input;
	size_t inbytesleft = input_len;
	
	char* outbuf = buffer;
	size_t outbytesleft = buffer_size;

	iconv_t cd = iconv_open (charset, "UCS-2LE");
	size_t niconv = iconv(cd, (const char**)&inbuf, &inbytesleft, (char**)(&outbuf), &outbytesleft);
	//OK
// 	if(inbytesleft == 0)
// 	{
// 		return buffer_size - outbytesleft;
// 	}
// 	//Fail
// 	else
// 	{
// 		return niconv;
// 	}

	return buffer_size - outbytesleft;
}

static char _hexcodes[] =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};
void hexDump(char* buffer, size_t size, std::string& s)
{
	s.erase();
	s.reserve(size * 2);
	size_t i;
	for(i = 0; i < size; i++)
	{
		if(i > 0)
		{
			s += ' ';
		}
		char ch = buffer[i];

		char lo = _hexcodes[ch & 0x0F];
		char hi = _hexcodes[(ch >> 4) & 0x0F];

		s += hi;
		s += lo;
	}
}

void test()
{
	const wchar_t* TEST_STRING = L"ÖÐÎÄ123";
	TEST_STRING = L"\x4E2D\x6587\x0031\x0032\x0033";
	std::wstring s(TEST_STRING);

	const int BUF_SIZE = 128;
	char buffer[BUF_SIZE];

	const char* CHARSETS[] = { "GB2312", "GBK", "ISO-8859-1", "US-ASCII", "UTF-16",
		"UTF-16BE", "UTF-16LE", "UTF-32", "UTF-32BE", "UTF-32LE", "UTF-8", "UTF-7" };
	size_t len = 0;
	int i;
	std::string output;

	for(i = 0; i < sizeof(CHARSETS)/sizeof(CHARSETS[0]); i++)
	{
		len = getBytes(s, buffer, BUF_SIZE, CHARSETS[i]);

		hexDump(buffer, len, output);

		printf("[%12s]:%s\n", CHARSETS[i], output.c_str());
	}
}




