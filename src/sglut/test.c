#include "sglutContext.h"
#include "utils.h"

int main(int argc, char *argv[])
{
  char* buf = malloc(512 * 512 * 4 * sizeof(char));
  int i = 0;
  Image* img = image_new_from_raw("lena.raw");
  sglutInit(argc, argv);
  for (i = 0; i < 512 * 512; ++i) {
    buf[i * 4 + 0] = img->buf[i * 3 + 2];
    buf[i * 4 + 1] = img->buf[i * 3 + 1];
    buf[i * 4 + 2] = img->buf[i * 3 + 0];
    buf[i * 4 + 3] = 0;
  }

  sglutBindBuffer(buf, buf);
  sglutInitWindowSize(512, 512);
  sglutInitWindowPosition(100, 100);
  sglutCreateWindow("Lena");
  sglutMainLoop();

  return 0;
}
