#include "sglutContext.h"

SGLUTContext g_sglut;

void _sglutRenderSingleFrame(void)
{
  static int buf_num = 0;
  XPutImage(g_sglut.display, g_sglut.window, g_sglut.gc,
            g_sglut.framebuffer_image[buf_num], 0, 0, 0, 0,
            g_sglut.width, g_sglut.height);
  buf_num = (++buf_num) % 2;
}

void sglutInit(int argc, char* argv[])
{
  g_sglut.display = XOpenDisplay(NULL);
}

void sglutInitWindowSize(int width, int height)
{
  int screen_number = XDefaultScreen(g_sglut.display);
  g_sglut.win_width = width;
  g_sglut.win_height = height;

  g_sglut.framebuffer[0] = malloc(width * height * 4 * sizeof(char));
  g_sglut.framebuffer[1] = malloc(width * height * 4 * sizeof(char));

  g_sglut.framebuffer_image[0] = XCreateImage(g_sglut.display,
      XDefaultVisual(g_sglut.display, screen_number),
      XDefaultDepth(g_sglut.display, screen_number),
      ZPixmap, 0, g_slugt.framebuffer_image[0], width, height, 32, 0);
  g_sglut.framebuffer_image[1] = XCreateImage(g_sglut.display,
      XDefaultVisual(g_sglut.display, screen_number),
      XDefaultDepth(g_sglut.display, screen_number),
      ZPixmap, 0, g_slugt.framebuffer_image[1], width, height, 32, 0);
}

void sglutInitWindowPosition(int x, int y)
{
  g_sglut.win_x = x;
  g_sglut.win_y = y;
}

void sglutCreateWindow(char* name)
{
  int blackColor = BlackPixel(g_slugt.display, DefaultScreen(g_slugt.display));
  g_sglut.window = XCreateSimpleWindow(g_sglut.display,
                                       DefaultRootWindow(g_sglut.display),
                                       g_sglut.x, g_sglut.y,
                                       g_sglut.width, g_sglut.height,
                                       0, blackColor, blackColor);
  XMapWindow(g_sglut.display, g_sglut.window);
  XStoreName(g_sglut.display, g_sglut.window, name);
  g_sglut.gc = XCreateGC(g_sglut.display, g_sglut.window, 0, NULL);
}

void sglutGetBuffer(char** buf1, char** buf2)
{
  buf1 = &g_sglut.framebuffer[0];
  buf2 = &g_sglut.framebuffer[1];
}

void sglutMainLoop(void)
{
  while (1) {
    _sglutRenderSingleFrame();
  }
}
