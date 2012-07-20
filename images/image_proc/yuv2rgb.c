#include <stdio.h>
#include <stdlib.h>

#include "fileoutput.h"
#include "image_proc.h"

/*
   yuv2rgb - Copyright 2007 by Michael Kohn
   http://www.mikekohn.net/  mike@mikekohn.net

   This code is NOT opensource and you may not use any of it
   in your own projects.

*/

int posix_memalign(void **memptr, size_t alignment, size_t size);

int main(int argc, char *argv[])
{
FILE *in,*out;
unsigned char *yuv_image=0;
unsigned char *rgb_image=0;
int width,height;
int x,y,ptr;

  printf("\nYUV422 to RGB - Copyright 2007 Michael Kohn\n"); 
  printf("Date Version: April 23, 2007\n\n");

  if (argc!=5)
  {
    printf("Usage: yuv2rgb <image.yuv422> <output image.bmp> <width> <height>\n");
    exit(0);
  }

  in=fopen(argv[1],"rb");
  if (in==0)
  {
    printf("Could not open input file: %s\n",argv[1]);
    exit(0);
  }

  out=fopen(argv[2],"wb");
  if (out==0)
  {
    printf("Could not open output file: %s\n",argv[2]);
    fclose(in);
    exit(0);
  }

  width=atoi(argv[3]);
  height=atoi(argv[4]);

  posix_memalign((void **)&yuv_image,16,width*height*2);
  posix_memalign((void **)&rgb_image,16,width*height*3);

  if (fread(yuv_image,1,width*height*2,in)!=width*height*2)
  {
    printf("Error: Couldn't read in full image?\n");
  }

if ((((long)yuv_image)%16)!=0)
{
  printf("not 16 byte aligned %ld\n",(((long)yuv_image)%16));
}

if ((((long)rgb_image)%16)!=0)
{
  printf("not 16 byte aligned %ld\n",(((long)rgb_image)%16));
}


  for (x=0; x<10000; x++)
  {
//unsigned int i;
    yuv2rgb_float(yuv_image,rgb_image,width,height);
    // yuv2rgb_int(yuv_image,rgb_image,width,height);
    // yuv2rgb_int_sat(yuv_image,rgb_image,width,height);
    // yuv2rgb_float_sse(yuv_image,rgb_image,width,height);
// printf("return value=%d %u (%u) mod4=%d\n",i,yuv_image+(704*480),yuv_image,i%4);
  }
  write_bmp_header(out,width,height);

  for (y=height-1; y>=0; y--)
  {
    ptr=y*(width*3);
    for (x=0; x<width; x++) 
    {
      write_bmp_pixel(out,rgb_image[ptr],rgb_image[ptr+1],rgb_image[ptr+2]);
      ptr=ptr+3;
    }
  }

  write_bmp_footer(out);
 
  free(yuv_image);
  free(rgb_image);

  fclose(in);
  fclose(out);

  printf("\n");

  return 0;
}

int posix_memalign(void **memptr, size_t alignment, size_t size)
{
	*memptr = malloc(size);

	return 0;
}

