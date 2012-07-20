#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileoutput.h"

/*

bmp.c - Copyright 2007 Michael Kohn (mike@mikekohn.net)
This falls under the Kohnian license.  Please read
it at http://ringtonetools.mikekohn.net/

This program is NOT opensourced.  You may not use any part
of this program for your own software.

*/

/* BMP */

int curr_x,total_x;

/* void write_bmp_header(FILE *out, int width, int height) */
void write_bmp_header(FILE *out, int width, int height)
{
int image_width;

  /* image_width=width*3; */
  image_width=width;
  while ((image_width%4)!=0) image_width++;
  total_x=width*3;

/* size: 14 bytes */

  write_chars(out,"BM");
  write_int(out,0);
  write_word(out,0);
  write_word(out,0);
  /* write_int(out,54+1024); */
  write_int(out,54);

/* head1: 14  head2: 40 */

  write_int(out,40);         /* biSize */
  write_int(out,width);
  write_int(out,height);
  write_word(out,1);
  write_word(out,24);
  /* write_word(out,8); */
  write_int(out,0);          /* compression */
  write_int(out,image_width*height);
  write_int(out,0);          /* biXPelsperMetre */
  write_int(out,0);          /* biYPelsperMetre */
  /* write_int(out,0); */
  /* write_int(out,0); */
  write_int(out,0);
  write_int(out,0);

/*
  for (t=0; t<256; t++)
  {
    write_int(out,palette[t]);
  }
*/

  curr_x=0;
}

void write_bmp_pixel(FILE *out, unsigned char red, unsigned char green, unsigned char blue)
{
  putc(blue,out);
  putc(green,out);
  putc(red,out);
  curr_x=curr_x+3;

  if (curr_x==total_x)
  {
    while ((curr_x%4)!=0)
    {
      putc(0,out);
      curr_x++;
    }
    curr_x=0;
  }
}

int write_bmp_footer(FILE *out)
{
int marker;

  marker=ftell(out);
  fseek(out,2,SEEK_SET);
  write_int(out,marker);
  fseek(out,marker,SEEK_SET);
  return 0;
}


