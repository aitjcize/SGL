#include "sglutContext.h"
#include <stdlib.h>

SGLUTContext g_sglut;

void _sglutRenderSingleFrame(void)
{
  static int buf_num = 0;
  XPutImage(g_sglut.display, g_sglut.window, g_sglut.gc,
            g_sglut.framebuffer_image[buf_num], 0, 0, 0, 0,
            g_sglut.win_width, g_sglut.win_height);
  XFlush(g_sglut.display);
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

  g_sglut.framebuffer_image[0] = XCreateImage(g_sglut.display,
      XDefaultVisual(g_sglut.display, screen_number),
      XDefaultDepth(g_sglut.display, screen_number),
      ZPixmap, 0, g_sglut.framebuffer[0], width, height, 32, 0);

  g_sglut.framebuffer_image[1] = XCreateImage(g_sglut.display,
      XDefaultVisual(g_sglut.display, screen_number),
      XDefaultDepth(g_sglut.display, screen_number),
      ZPixmap, 0, g_sglut.framebuffer[1], width, height, 32, 0);
}

void sglutInitWindowPosition(int x, int y)
{
  g_sglut.win_x = x;
  g_sglut.win_y = y;
}

void sglutCreateWindow(char* name)
{
  XEvent e;
  int blackColor = BlackPixel(g_sglut.display, DefaultScreen(g_sglut.display));

  g_sglut.window = XCreateSimpleWindow(g_sglut.display,
                                       DefaultRootWindow(g_sglut.display),
                                       g_sglut.win_x, g_sglut.win_y,
                                       g_sglut.win_width, g_sglut.win_height,
                                       0, blackColor, blackColor);

  /* Select Input Mask */
  XSelectInput(g_sglut.display, g_sglut.window,
      StructureNotifyMask | ExposureMask | KeyPressMask | ButtonPressMask);

  XMapWindow(g_sglut.display, g_sglut.window);
  do {
    XNextEvent(g_sglut.display, &e);
  } while (e.type != MapNotify);

  XStoreName(g_sglut.display, g_sglut.window, name);
  g_sglut.gc = XCreateGC(g_sglut.display, g_sglut.window, 0, NULL);
}

void sglutBindBuffer(char* buf1, char* buf2)
{
  g_sglut.framebuffer[0] = buf1;
  g_sglut.framebuffer[1] = buf2;
}

void sglutDisplayFunc(void (*func)(void))
{
  g_sglut.displayFunc = func;
}

void sglutKeyboardFunc(void (*func)(unsigned char key, int x, int y))
{
  g_sglut.keyboardFunc = func;
}

void sglutMouseFunc(void (*func)(int button, int state, int x, int y))
{
  g_sglut.mouseFunc = func;
}

void sglutMainLoop(void)
{
  XEvent report;
  while (1) {
    XNextEvent(g_sglut.display, &report);
    _sglutRenderSingleFrame();
    switch (report.type) {
    case Expose:
      if (g_sglut.displayFunc)
        g_sglut.displayFunc();
      break;
    case KeyPress:
      if (g_sglut.keyboardFunc)
        g_sglut.keyboardFunc(report.xkey.keycode, report.xkey.x, report.xkey.y);
      break;
    case ButtonPress:
      if (g_sglut.keyboardFunc)
        g_sglut.mouseFunc(report.xbutton.button, report.xbutton.state,
                          report.xbutton.x, report.xbutton.y);
      break;
    }
  }
}
