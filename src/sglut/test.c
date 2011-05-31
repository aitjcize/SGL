#include "sglut.h"
#include "utils.h"

void motion(int x, int y) {
  printf("(%d, %d)\n", x, y);
}

int main(int argc, char *argv[])
{
  char* buf = malloc(512 * 512 * 4 * sizeof(char));
  int i = 0;
  Image* img = image_new_from_raw("lena.raw");
  glutInit(argc, argv);
  for (i = 0; i < 512 * 512; ++i) {
    buf[i * 4 + 0] = img->buf[i * 3 + 2];
    buf[i * 4 + 1] = img->buf[i * 3 + 1];
    buf[i * 4 + 2] = img->buf[i * 3 + 0];
    buf[i * 4 + 3] = 0;
  }

  glutBindBuffer(buf, buf);
  glutInitWindowSize(512, 512);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Lena");
  glutMotionFunc(motion);
  glutMainLoop();

  return 0;
}
