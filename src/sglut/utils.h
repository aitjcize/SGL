/*
 * utils.h
 */

#define CLAMP(x, low, high) (((x) > (high))?(high): (((x) < (low))?(low): (x)))

typedef struct _Image Image;

struct _Image {
  int len;
  unsigned char* buf;
};

Image* image_new_from_raw(const char* filename);
Image* image_new_with_size(int len);
void image_destroy(Image* img);
void image_dump_raw(Image* img, char* name, char* suffix);
