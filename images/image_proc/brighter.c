#include <stdio.h>
#include <stdlib.h>

#include "fileoutput.h"
#include "image_proc.h"

/*
   Brighter - Copyright 2007 by Michael Kohn
   http://www.mikekohn.net/  mike@mikekohn.net

   This code is NOT opensource and you may not use any of it
   in your own projects.

*/

int main(int argc, char *argv[])
{
FILE *in,*out;
struct _pic_info pic_info;
unsigned char *ubw_image;
unsigned char *bw_image;
int bright_value;
// int contrast_value;
int image_len;
int x,y,ptr;

  printf("\nBrightness - Copyright 2007 Michael Kohn\n"); 
  printf("Date Version: March 27, 2007\n\n");

  if (argc!=4)
  {
    printf("Usage: brightness <image.bmp> <output image.bmp> <brightness [-128 to 127]>\n");
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

  bright_value=atoi(argv[3]);
  // contrast_value=atoi(argv[4]);

  pic_info.picture=0;

  parse_bmp(in,&pic_info);

  image_len=pic_info.width*pic_info.height;

  ubw_image=(unsigned char *)malloc(image_len+15);

  if ((((long)ubw_image)%16)!=0)
  {
    bw_image=ubw_image+(16-((long)ubw_image%16));
// printf("%ld %ld\n",ubw_image,bw_image);
  }
    else
  {
    bw_image=ubw_image;
  }

/*
if ((((long)bw_image)%16)!=0)
{
  printf("not 16 byte aligned %ld\n",(((long)bw_image)%16));
}
*/

  for (x=0; x<image_len; x++)
  {
    bw_image[x]=((pic_info.picture[x]&255)+
                ((pic_info.picture[x]>>8)&255)+
                ((pic_info.picture[x]>>16)&255))/3;
  }


  printf("Adjusting brightness %d\n",bright_value);


  // for (x=0; x<100000; x++)
  {
    // if (bright_value!=0) brightness(bw_image,image_len,bright_value);
    // if (contrast_value!=0) contrast(bw_image,image_len,contrast_value);
    if (bright_value!=0) brightness(bw_image,image_len,bright_value);
    // if (contrast_value!=0) contrast_sse(bw_image,image_len,contrast_value);
  }

  write_bmp_header(out,pic_info.width,pic_info.height);

  for (y=pic_info.height-1; y>=0; y--)
  {
    ptr=y*pic_info.width;
    for (x=0; x<pic_info.width; x++) 
    {
      write_bmp_pixel(out,bw_image[ptr],bw_image[ptr],bw_image[ptr]);
      ptr++;
    }
  }


  write_bmp_footer(out);
 
  free(ubw_image);
  if (pic_info.picture!=0) free(pic_info.picture);

  fclose(in);
  fclose(out);

  printf("\n");

  return 0;
}

