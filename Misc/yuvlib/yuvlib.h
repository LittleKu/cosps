#ifndef _YUV_LIB_H_
#define _YUV_LIB_H_

#ifndef int8_t
#define int8_t   char
#endif

#ifndef uint8_t
#define uint8_t  unsigned char
#endif

#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef max
#define max(a,b) (((a)>(b))?(a):(b))
#endif

class CYuvUtils
{
public:
	static void YUV444toRGB888(uint8_t y, uint8_t u, uint8_t v, uint8_t* rgbBuffer);

	static int RGB888toRGB24(uint8_t *rgb888, int rgbSize, int width, int height);

	static void Flip(uint8_t* rgb_buf, int rgbSize, int width, int height);

	static int YV12_to_RGB24(uint8_t* y_buf, uint8_t* u_buf, uint8_t* v_buf, 
		uint8_t* rgb_buf, int rgbSize, int width, int height);
};

#endif