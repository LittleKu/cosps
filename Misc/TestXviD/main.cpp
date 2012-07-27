#include "xvid.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "yuvlib.h"
#include <windows.h>
#include "XvidEncoder.h"

#define OUTPUT_FILE		stdout
int XDIM = 0;
int YDIM = 0;

void test_xvid_init();
void test_pgm_to_bmp(const char* filename);

int read_pgmheader(FILE * handle);
int read_pgmdata(FILE * handle, unsigned char *image);
int save_to_bmp(BITMAPINFOHEADER *lpBMIH, BYTE *lpBuffer, DWORD cbBuffer, LPCTSTR lpszFileName);


int main(int argc, char* argv[])
{	
	test_xvid_init();
	test_pgm_to_bmp("cactus.pgm");
	printf("\n");
	return 0;
}

void test_xvid_init()
{
	xvid_gbl_init_t xvid_gbl_init;
	
	/*------------------------------------------------------------------------
	* Xvid core initialization
	*----------------------------------------------------------------------*/
	
	/* Set version -- version checking will done by xvidcore */
	memset(&xvid_gbl_init, 0, sizeof(xvid_gbl_init));
	xvid_gbl_init.version = XVID_VERSION;
    xvid_gbl_init.debug = XVID_DEBUG_ERROR;
	xvid_global(NULL, XVID_GBL_INIT, &xvid_gbl_init, NULL);

	xvid_gbl_info_t xvid_gbl_info;
	
	/* Clear xvid_gbl_info with 0s */
	memset(&xvid_gbl_info, 0, sizeof(xvid_gbl_info_t));
	/* Version */
	xvid_gbl_info.version = XVID_VERSION;
	/* Take effect */
	xvid_global(NULL, XVID_GBL_INFO, &xvid_gbl_info, NULL);
	
	/* print */
	printf("actual version: %d.%d.%d\n",
		XVID_VERSION_MAJOR(xvid_gbl_info.actual_version),
		XVID_VERSION_MINOR(xvid_gbl_info.actual_version),
		XVID_VERSION_PATCH(xvid_gbl_info.actual_version));
	
	fprintf(OUTPUT_FILE, "Detected CPU flags: ");
	if (xvid_gbl_info.cpu_flags & XVID_CPU_ASM)
		fprintf(OUTPUT_FILE, "ASM ");
	if (xvid_gbl_info.cpu_flags & XVID_CPU_MMX)
		fprintf(OUTPUT_FILE, "MMX ");
	fprintf(OUTPUT_FILE, "\n");
	
	if(xvid_gbl_info.build)
	{
		fprintf(OUTPUT_FILE, "build: %s\n", xvid_gbl_info.build);
	}
	fprintf(OUTPUT_FILE, "threads: %d\n", xvid_gbl_info.num_threads);
}

void test_pgm_to_bmp(const char* filename)
{
	FILE* fp = fopen(filename, "rb");
	if(fp == NULL)
	{
		fprintf(OUTPUT_FILE, "can't open file %s", filename);
		return;
	}

	unsigned char* image_buf = 0;
	unsigned char* rgb24_buf = 0;

	BITMAPINFOHEADER bmpInfoHdr;
	ZeroMemory(&bmpInfoHdr, sizeof(bmpInfoHdr));
	bmpInfoHdr.biSize = sizeof(bmpInfoHdr);
	bmpInfoHdr.biPlanes = 1;
	bmpInfoHdr.biBitCount = 24;
	bmpInfoHdr.biCompression = BI_RGB;

	int ret = 0;
	char frame_file[MAX_PATH];
	int frameSize = 0;

	CXvidEncoder xvidEncoder;

	FILE* fp_out = fopen("out.mp4", "w+b");

	for(int i = 0; i < 3; i++)
	{
		ret = read_pgmheader(fp);
		if(ret != 0)
		{
			fprintf(OUTPUT_FILE, "read_pgmheader failed: %d", ret);
			break;
		}

		bmpInfoHdr.biWidth = XDIM;
		bmpInfoHdr.biHeight = YDIM;
		bmpInfoHdr.biSizeImage = XDIM * YDIM * 3;

		if(!xvidEncoder.IsEncoderCreated())
		{
			ret = xvidEncoder.CreateEncoder(XDIM, YDIM);
			if(ret < 0)
			{
				fprintf(OUTPUT_FILE, "CreateEncoder failed: %d", ret);
				break;
			}
			ret = 0;
		}

		image_buf = new unsigned char[XDIM * YDIM * 3 / 2];

		ret = read_pgmdata(fp, image_buf);
		if(ret != 0)
		{
			fprintf(OUTPUT_FILE, "read_pgmdata failed: %d", ret);
			break;
		}

		frameSize = xvidEncoder.EncodeFrame(image_buf, XDIM, XVID_CSP_I420, 6);
		if(frameSize <= 0)
		{
			fprintf(OUTPUT_FILE, "EncodeFrame failed: %d", frameSize);
			break;
		}
		sprintf(frame_file, "%s%03d.m4v", "frame", i);
		{
			FILE *out = fopen(frame_file, "w+b");
			if(fwrite(xvidEncoder.GetEncodedFrame(), 1, frameSize, out) != frameSize)
			{
				fprintf(OUTPUT_FILE, "WriteFrame failed: %d", frameSize);
				fclose(out);
				break;
			}
			fclose(out);
		}
		if(fp_out != NULL)
		{
			if(fwrite(xvidEncoder.GetEncodedFrame(), 1, frameSize, fp_out) != frameSize)
			{
				fprintf(OUTPUT_FILE, "WriteFrame to fp_out failed: %d", frameSize);
				break;
			}
		}
		

		if(rgb24_buf == 0)
		{
			rgb24_buf = new unsigned char[XDIM * YDIM * 3];
		}


		ret = CYuvUtils::YV12_to_RGB24(image_buf, image_buf + XDIM * YDIM, image_buf + XDIM * YDIM + XDIM / 2 * YDIM / 2,
			rgb24_buf, XDIM * YDIM * 3, XDIM, YDIM);
		if(ret != 0)
		{
			fprintf(OUTPUT_FILE, "YV12_to_RGB24 failed: %d", ret);
			break;
		}
		
		sprintf(frame_file, "%s%03d.bmp", "frame", i);
		ret = save_to_bmp(&bmpInfoHdr, rgb24_buf, XDIM * YDIM * 3, frame_file);
		if(ret != 0)
		{
			fprintf(OUTPUT_FILE, "save_to_bmp failed: %d", ret);
			break;
		}

		delete image_buf;
		image_buf = 0;
	}

	if(image_buf != 0)
	{
		delete image_buf;
		image_buf = 0;
	}
	if(rgb24_buf != 0)
	{
		delete rgb24_buf;
		rgb24_buf = 0;
	}

	fclose(fp);

	if(fp_out != NULL)
	{
		fclose(fp_out);
	}
}

int read_pgmheader(FILE * handle)
{
	int bytes, xsize, ysize, depth;
	char dummy[2];
	
	bytes = (int) fread(dummy, 1, 2, handle);
	
	if ((bytes < 2) || (dummy[0] != 'P') || (dummy[1] != '5'))
		return (1);
	
	fscanf(handle, "%d %d %d", &xsize, &ysize, &depth);
	if ((xsize > 4096) || (ysize > 4096*3/2) || (depth != 255)) {
		fprintf(stderr, "%d %d %d\n", xsize, ysize, depth);
		return (2);
	}
	if ((XDIM == 0) || (YDIM == 0)) {
		XDIM = xsize;
		YDIM = ysize * 2 / 3;
	}

	while(true)
	{
		if(fread(dummy, 1, 1, handle) != 1)
		{
			return 4;
		}
		if(dummy[0] == '\n' || dummy[0] == '\r' || dummy[0] == ' ' || dummy[0] == '\t')
		{
			break;
		}
	}
	
	return (0);
}
int read_pgmdata(FILE * handle, unsigned char *image)
{
	int i;
//	char dummy;
	
	unsigned char *y = image;
	unsigned char *u = image + XDIM * YDIM;
	unsigned char *v = image + XDIM * YDIM + XDIM / 2 * YDIM / 2;
	
	/* read Y component of picture */
	if(fread(y, 1, XDIM * YDIM, handle) != (size_t)(XDIM * YDIM))
	{
		return 1;
	}
	
	for (i = 0; i < YDIM / 2; i++) {
		/* read U */
		if(fread(u, 1, XDIM / 2, handle) != (size_t)(XDIM / 2))
		{
			return 2;
		}
		
		/* read V */
		if(fread(v, 1, XDIM / 2, handle) != (size_t)(XDIM / 2))
		{
			return 3;
		}
		
		/* Update pointers */
		u += XDIM / 2;
		v += XDIM / 2;
	}
	
	/*  I don't know why, but this seems needed */
// 	if(fread(&dummy, 1, 1, handle) != 1)
// 	{
// 		return 4;
// 	}
	
	return (0);
}

int save_to_bmp(BITMAPINFOHEADER *lpBMIH, BYTE *lpBuffer, DWORD cbBuffer, LPCTSTR lpszFileName)
{
	int ret = 0;
	BITMAPFILEHEADER bmpFileHdr;
	bmpFileHdr.bfType = 0x4D42;
	bmpFileHdr.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmpFileHdr.bfReserved1 = bmpFileHdr.bfReserved2 = 0;
	bmpFileHdr.bfSize = bmpFileHdr.bfOffBits + cbBuffer;
	
	
	HANDLE hFile = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
    {
		return 1;
    }
	
	HRESULT hr = E_FAIL;
	DWORD dwWrite = 0;
	do 
	{
		if(!WriteFile(hFile, &bmpFileHdr, sizeof(BITMAPFILEHEADER), &dwWrite, NULL))
		{
			ret = 2;
			break;
		}
		
		if(!WriteFile(hFile, lpBMIH, sizeof(BITMAPINFOHEADER), &dwWrite, NULL))
		{
			ret = 3;
			break;
		}
		if(!WriteFile(hFile, lpBuffer, cbBuffer, &dwWrite, NULL))
		{
			ret = 4;
			break;
		}
		
		hr = S_OK;
		
	} while (FALSE);
	
	CloseHandle(hFile);
	
	return ret;
}