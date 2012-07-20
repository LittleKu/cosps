#include <stdio.h>
#include <stdlib.h>

#include "fileoutput.h"

/*

Stereogram - Copyright 2004 Michael Kohn (mike@mikekohn.net)
This falls under the Kohnian license.  Please read
it at http://ringtonetools.mikekohn.net/

This program is NOT opensourced.  You may not use any part
of this program for your own software.

*/

int write_int(FILE *out, int n)
{
  putc((n&255),out);
  putc(((n>>8)&255),out);
  putc(((n>>16)&255),out);
  putc(((n>>24)&255),out);

  return 0;
}

int write_word(FILE *out, int n)
{
  putc((n&255),out);
  putc(((n>>8)&255),out);

  return 0;
}

int read_int(FILE *in)
{
int c;

  c=getc(in);
  c=c|(getc(in)<<8);
  c=c|(getc(in)<<16);
  c=c|(getc(in)<<24);

  return c;
}

int read_word(FILE *in)
{
int c;

  c=getc(in);
  c=c|(getc(in)<<8);

  return c;
}


int read_chars(FILE *in, char *s, int count)
{
int t;

  for (t=0; t<count; t++)
  {
    s[t]=getc(in);
  }

  s[t]=0;

  return 0;
}

int write_chars(FILE *out, char *s)
{
int t;

  t=0;
  while(s[t]!=0 && t<255)
  {
    putc(s[t++],out);
  }

  return 0;
}

