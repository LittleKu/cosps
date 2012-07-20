#include <stdio.h>
#include <stdlib.h>

#include "fileoutput.h"

/* 

parse_bmp.c - Copyright 2007 Michael Kohn (mike@mikekohn.net)
This falls under the Kohnian license.

This program is NOT opensourced.  You may not use any part
of this program for your own software.

*/

void read_bitmap_file(FILE *in, struct _bitmap_file *bitmap_file)
{
  bitmap_file->bfType[0]=getc(in);
  bitmap_file->bfType[1]=getc(in);
  bitmap_file->bfSize=read_int(in);
  bitmap_file->reserved1=read_word(in);
  bitmap_file->reserved2=read_word(in);
  bitmap_file->bfOffs=read_int(in);
}

void read_bitmap_info(FILE *in, struct _bitmap_info *bitmap_info)
{
  bitmap_info->biSize=read_int(in);
  bitmap_info->biWidth=read_int(in);
  bitmap_info->biHeight=read_int(in);
  bitmap_info->biPlanes=read_word(in);
  bitmap_info->biBitCount=read_word(in);
  bitmap_info->biCompression=read_int(in);
  bitmap_info->biSizeImage=read_int(in);
  bitmap_info->biXPelsPerMetre=read_int(in);
  bitmap_info->biYPelsPerMetre=read_int(in);
  bitmap_info->biClrUsed=read_int(in);
  bitmap_info->biClrImportant=read_int(in);
}

void raw_uncompressed(FILE *in, unsigned int *picture, struct _bitmap_info *bitmap_info)
{
int x,y;
int c=0,t;
int byte_count;

  for (y=bitmap_info->biHeight-1; y>=0; y--)
  {
    byte_count=0;

    for (x=0; x<bitmap_info->biWidth; x++)
    {
      if (bitmap_info->biBitCount==8 || bitmap_info->biBitCount==24 || bitmap_info->biBitCount==32)
      {
        if (bitmap_info->biBitCount==8) 
        {
          c=getc(in);
          c=bitmap_info->colors[c];
/* printf("0x%06x\n",c); */
          byte_count++;
        }
          else
        if (bitmap_info->biBitCount==24)
        {
          c=getc(in)+(getc(in)<<8)+(getc(in)<<16);
          byte_count=byte_count+3;
        }
          else
        if (bitmap_info->biBitCount==32)
        {
          c=getc(in)+(getc(in)<<8)+(getc(in)<<16);
          t=getc(in);
          /* if (t==255 && bitmap_info->trans==-2) bitmap_info->trans=c; */

          byte_count=byte_count+4;
        }

        picture[x+(y*bitmap_info->biWidth)]=c;
      }
        else
      if (bitmap_info->biBitCount==4)
      {
        c=getc(in);
        byte_count++;

        picture[x+(y*bitmap_info->biWidth)]=bitmap_info->colors[((c>>4)&15)];
        x++;

        if (x<bitmap_info->biWidth)
        {
          picture[x+(y*bitmap_info->biWidth)]=bitmap_info->colors[(c&15)];
        }
      }
        else
      if (bitmap_info->biBitCount==1)
      {
        c=getc(in);
        byte_count++;

        for (t=7; t>=0; t--)
        {
          if (x<bitmap_info->biWidth)
          {
            if (((c>>t)&1)==0)
            { picture[x+(y*bitmap_info->biWidth)]=bitmap_info->colors[0]; }
              else
            { picture[x+(y*bitmap_info->biWidth)]=bitmap_info->colors[1]; }
          }
          x++;
        }
        x=x-1;
      }
    }

    c=(byte_count%4);
    if (c!=0)
    {
      for (t=c; t<4; t++)
      { getc(in); }
    }
  }
}

void raw_compressed(FILE *in, unsigned int *picture, struct _bitmap_info *bitmap_info)
{
int x,y;
int c,t,r;

  y=bitmap_info->biHeight-1;
  x=0;

  while (1)
  {
#ifdef DEBUG
if (x==0) printf("reading line %d\n",y);
#endif

    c=getc(in);

    if (c==EOF) return;

    if (c!=0)
    {
      r=getc(in);
#ifdef DEBUG
printf("repeat same %d %d times\n",r,c);
#endif

      for (t=0; t<c; t++)
      {
        if (bitmap_info->biBitCount==4)
        {
          if ((t%2)==0)
          { picture[x+(y*bitmap_info->biWidth)]=bitmap_info->colors[(r>>4)]; }
            else
          { picture[x+(y*bitmap_info->biWidth)]=bitmap_info->colors[(r&15)]; }
        }
          else
        if (bitmap_info->biBitCount==8)
        { picture[x+(y*bitmap_info->biWidth)]=bitmap_info->colors[r]; }

        x++;
      }
    }
      else
    {
      r=getc(in);
#ifdef DEBUG
printf("repeat not the same %d times\n",r);
#endif

      if (r==0)
      {
        x=0;
        y--;
        continue;
      }
        else
      if (r==1)
      { break; }
        else
      if (r==2)
      {
        x=x+getc(in);
        y=y-getc(in);
        return;
      }

      for (t=0; t<r; t++)
      {
        c=getc(in);
        if (bitmap_info->biBitCount==8)
        {
          picture[x+(y*bitmap_info->biWidth)]=bitmap_info->colors[c];
        }
          else
        if (bitmap_info->biBitCount==4)
        {
          picture[x+(y*bitmap_info->biWidth)]=bitmap_info->colors[c>>4];
          t++;
          if (t<r)
          {
            x++;
            picture[x+(y*bitmap_info->biWidth)]=bitmap_info->colors[(c&15)];
          }
        }

        x++;
      }

      if (bitmap_info->biBitCount==8)
      { c=r%2; }
        else
      if (bitmap_info->biBitCount==4)
      {
        t=(r/2)+(r%2);
        c=t%2;
      }

      if (c!=0)
      { getc(in); }
    }
  }
}

int parse_bmp(FILE *in, struct _pic_info *pic_info)
{
struct _bitmap_file bitmap_file;
struct _bitmap_info bitmap_info;
int t;
/* int t,c; */

  read_bitmap_file(in, &bitmap_file);

  if (bitmap_file.bfType[0]!='B' || bitmap_file.bfType[1]!='M')
  {
    printf("Not a bitmap.\n");
    return -1;
  }

  read_bitmap_info(in, &bitmap_info);

#ifdef DEBUG
  printf("Bitmap File Header\n");
  printf("----------------------------------------------\n");
  printf("         bfType: %c%c\n",bitmap_file.bfType[0],bitmap_file.bfType[1]);
  printf("         bfSize: %d\n",bitmap_file.bfSize);
  printf("      reserved1: %d\n",bitmap_file.reserved1);
  printf("      reserved2: %d\n",bitmap_file.reserved2);
  printf("         bfOffs: %d\n",bitmap_file.bfOffs);
  printf("----------------------------------------------\n");
  printf("Bitmap Info Header\n");
  printf("----------------------------------------------\n");
  printf("         biSize: %d\n",bitmap_info.biSize);
  printf("        biWidth: %d\n",bitmap_info.biWidth);
  printf("       biHeight: %d\n",bitmap_info.biHeight);
  printf("       biPlanes: %d\n",bitmap_info.biPlanes);
  printf("     biBitCount: %d\n",bitmap_info.biBitCount);
  printf("  biCompression: %d\n",bitmap_info.biCompression);
  printf("    biSizeImage: %d\n",bitmap_info.biSizeImage);
  printf("biXPelsPerMetre: %d\n",bitmap_info.biXPelsPerMetre);
  printf("biYPelsPerMetre: %d\n",bitmap_info.biYPelsPerMetre);
  printf("      biClrUsed: %d\n",bitmap_info.biClrUsed);
  printf(" biClrImportant: %d\n",bitmap_info.biClrImportant);
  printf("----------------------------------------------\n");
#endif

  bitmap_info.colors[0]=0;
  bitmap_info.colors[1]=0xffffff;
  bitmap_info.colors[255]=0xffffff;

  if (bitmap_info.biClrImportant==0 && bitmap_info.biBitCount==8)
  { bitmap_info.biClrImportant=256; }
  
  for (t=0; t<bitmap_info.biClrImportant; t++)
  {
    bitmap_info.colors[t]=read_int(in);
/*
    c=(getc(in)+(getc(in)<<8)+(getc(in)<<16));
    getc(in);
    colors[t]=c;
*/
  }

  pic_info->width=bitmap_info.biWidth;
  pic_info->height=bitmap_info.biHeight;

  pic_info->picture=malloc(bitmap_info.biWidth*bitmap_info.biHeight*sizeof(int));
  // posix_memalign((void **)&pic_info->picture,16,bitmap_info.biWidth*bitmap_info.biHeight*sizeof(int));

  fseek(in,bitmap_file.bfOffs,0);

  if (bitmap_info.biCompression==0)
  {
    raw_uncompressed(in,pic_info->picture,&bitmap_info);
  }
    else
  if (bitmap_info.biCompression==1)
  {
    raw_compressed(in,pic_info->picture,&bitmap_info);
  }
    else
  if (bitmap_info.biCompression==2)
  {
    raw_compressed(in,pic_info->picture,&bitmap_info);
  }
    else
  if (bitmap_info.biCompression==3)
  {
    raw_uncompressed(in,pic_info->picture,&bitmap_info);
  }
    else
  {
    printf("This type of compression is not supported at this time.\n");
    return 0;
  }

  return 0;
}

