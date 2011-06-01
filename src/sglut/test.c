#include "sglut.h"
#include "utils.h"

#include <stdio.h>

void motion(int x, int y) {
  printf("(%d, %d)\n", x, y);
}

void render(char* framebuffer)
{
  Image* img = image_new_from_raw("lena.raw");
  int i = 0;
  for (i = 0; i < 512 * 512; ++i) {
    framebuffer[i * 4 + 0] = img->buf[i * 3 + 2];
    framebuffer[i * 4 + 1] = img->buf[i * 3 + 1];
    framebuffer[i * 4 + 2] = img->buf[i * 3 + 0];
    framebuffer[i * 4 + 3] = 0;
  }
  image_destroy(img);
}

int main(int argc, char *argv[])
{
  glutInit(argc, argv);
  glutSetRenderApp(render);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(512, 512);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Lena");
  glutSetWindowTitle("Test");
  glutMotionFunc(motion);
  glutMainLoop();

  return 0;
}
