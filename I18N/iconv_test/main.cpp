#include <stdio.h>
#include <string.h>
#include <iconv.h>

bool bDebug = true;

int convert(const char* in_file, const char* in_encoding, const char* out_file, const char* out_encoding);

int main(int argc, char* argv[])
{
	int rc = convert("input.txt", "UTF-8", "output.txt", "UCS-2BE");
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




