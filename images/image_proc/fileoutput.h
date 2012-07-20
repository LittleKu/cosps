

int write_int(FILE *out, int n);
int write_word(FILE *out, int n);
int read_int(FILE *in);
int read_word(FILE *in);

int read_chars(FILE *in, char *s, int count);
int write_chars(FILE *out, char *s);

struct _pic_info
{
  int width;
  int height;
  unsigned int *picture;
};

struct _bitmap_file
{
  unsigned char bfType[2];
  unsigned int bfSize;
  unsigned short int reserved1;
  unsigned short int reserved2;
  unsigned int bfOffs;
};

struct _bitmap_info
{
  unsigned int biSize;
  unsigned int biWidth;
  unsigned int biHeight;
  unsigned short int biPlanes;
  unsigned short int biBitCount;
  unsigned int biCompression;
  unsigned int biSizeImage;
  unsigned int biXPelsPerMetre;
  unsigned int biYPelsPerMetre;
  unsigned int biClrUsed;
  unsigned int biClrImportant;
  int colors[256];
};

void read_bitmap_file(FILE *in, struct _bitmap_file *bitmap_file);
void read_bitmap_info(FILE *in, struct _bitmap_info *bitmap_info);
void write_bmp_header(FILE *out, int width, int height);
void write_bmp_pixel(FILE *out, unsigned char red, unsigned char green, unsigned char blue);
int write_bmp_footer(FILE *out);

/* int write_bmp_pixels(FILE *out, unsigned int *picture, int width, int height); */

int parse_bmp(FILE *in, struct _pic_info *pic_info);

/*
void raw_uncompressed(FILE *out, FILE *in0, FILE *in1, struct _bitmap_info *bitmap_info);
void raw_compressed8(FILE *out, FILE *in0, FILE *in1, struct _bitmap_info *bitmap_info);
*/

#ifdef DEBUG
void display_bmp_info(int count, struct _bitmap_file *bitmap_file, struct _bitmap_info *bitmap_info);
#endif

