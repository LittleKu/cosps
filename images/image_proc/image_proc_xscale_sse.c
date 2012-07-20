#include <stdio.h>
#include <stdlib.h>
#include <mmintrin.h>

void brightness_sse(unsigned char *buffer, int len, int v)
{
__m64 pixel_vector;
__m64 value_vector;
int t;

printf("brightness\n");
fflush(stdout);

  if (v>0)
  {
    // value_vector=_mm_set1_pi8(v);
printf("set some stuff\n");
fflush(stdout);
    value_vector=_mm_set_pi16(v,v,v,v);
printf("DONE: set some stuff\n");
fflush(stdout);

    for (t=0; t<len; t=t+8)
    {
printf("load a register %d\n",t);
fflush(stdout);
      // pixel_vector=*((__m64 *)(buffer+t));
      pixel_vector=_mm_cvtsi64_m64(*((__int64 *)(buffer+t)));
printf("add\n");
fflush(stdout);
      // pixel_vector=_mm_adds_pu8(pixel_vector,value_vector); 
printf("write back\n");
fflush(stdout);
      *((__m64 *)(buffer+t))=pixel_vector;
printf("done\n");
fflush(stdout);
    }
  }
    else
  {
    v=-v;
    // v=(v<<8)+v;
    // value_vector=_mm_set_pi16(v,v,v,v);
    value_vector=_mm_set1_pi8(v);

    for (t=0; t<len; t=t+8)
    {
      // pixel_vector=*((__m64 *)(buffer+t));
      pixel_vector=_mm_cvtsi64_m64(*((__int64 *)(buffer+t)));
      pixel_vector=_mm_subs_pu8(pixel_vector,value_vector); 
      *((__m64 *)(buffer+t))=pixel_vector;
    }
  }
}

void contrast_sse(unsigned char *buffer, int len, int v)
{

}


