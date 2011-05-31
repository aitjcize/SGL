#include "sglutContext.h"
#include <stdio.h>
#include <stdlib.h>

SGLUTContext g_sglut;

void _sglutRenderSingleFrame(void)
{
  XPutImage(g_sglut.display, g_sglut.window, g_sglut.gc,
            g_sglut.framebuffer_image[g_sglut.buf_index], 0, 0, 0, 0,
            g_sglut.win_width, g_sglut.win_height);
  XFlush(g_sglut.display);
  g_sglut.buf_index = (++g_sglut.buf_index) % 2;
}

void sglutInit(int argc, char* argv[])
{
  g_sglut.display = XOpenDisplay(NULL);
  g_sglut.flags = 0;

  int i = 0;
  for (i = 1; i < argc; ++i)
    if (strcmp(argv[i], "-d") == 0)
      g_sglut.flags |= SGLUT_DEBUG;
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

  if (!g_sglut.framebuffer_image[0]) {
    fprintf(stderr, "Can't not create XImage.\n");
    exit(1);
  }

  g_sglut.framebuffer_image[1] = XCreateImage(g_sglut.display,
      XDefaultVisual(g_sglut.display, screen_number),
      XDefaultDepth(g_sglut.display, screen_number),
      ZPixmap, 0, g_sglut.framebuffer[1], width, height, 32, 0);

  if (!g_sglut.framebuffer_image[1]) {
    fprintf(stderr, "Can't not create XImage.\n");
    exit(1);
  }
}

void sglutInitWindowPosition(int x, int y)
{
  g_sglut.win_x = x;
  g_sglut.win_y = y;
}

void sglutCreateWindow(char* name)
{
  int blackColor = BlackPixel(g_sglut.display, DefaultScreen(g_sglut.display));

  g_sglut.window = XCreateSimpleWindow(g_sglut.display,
                                       DefaultRootWindow(g_sglut.display),
                                       g_sglut.win_x, g_sglut.win_y,
                                       g_sglut.win_width, g_sglut.win_height,
                                       0, blackColor, blackColor);

  // register interest in the delete window message
  Atom wmDeleteMessage = XInternAtom(g_sglut.display, "WM_DELETE_WINDOW",False);
  XSetWMProtocols(g_sglut.display, g_sglut.window, &wmDeleteMessage, 1);

  /* Select Input Mask */
  XSelectInput(g_sglut.display, g_sglut.window,
      StructureNotifyMask | ExposureMask | KeyPressMask | ButtonPressMask);

  XMapWindow(g_sglut.display, g_sglut.window);
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

void sglutSwapBuffers(void)
{
  _sglutRenderSingleFrame();
}

void sglutDestroy(void)
{
  XFreeGC(g_sglut.display, g_sglut.gc);
  XDestroyWindow(g_sglut.display, g_sglut.window);
  XCloseDisplay(g_sglut.display);
}

void sglutMainLoop(void)
{
  XEvent event;
  while (1) {
    XNextEvent(g_sglut.display, &event);
    _sglutRenderSingleFrame();
    switch (event.type) {
    case Expose:
      if (g_sglut.displayFunc)
        g_sglut.displayFunc();
      if (g_sglut.flags & SGLUT_DEBUG)
        fprintf(stderr, "ExposeEvent\n");
      break;

    case KeyPress:
      if (g_sglut.keyboardFunc)
        g_sglut.keyboardFunc(event.xkey.keycode, event.xkey.x, event.xkey.y);
      if (g_sglut.flags & SGLUT_DEBUG)
        fprintf(stderr, "KeyPressEvent: (%d, %d, %d)\n",
                       event.xkey.keycode, event.xkey.x, event.xkey.y);
      break;

    case ButtonPress:
      if (g_sglut.keyboardFunc)
        g_sglut.mouseFunc(event.xbutton.button, event.xbutton.state,
                          event.xbutton.x, event.xbutton.y);
      if (g_sglut.flags & SGLUT_DEBUG)
        fprintf(stderr, "ButtonPressEvent: (%d, %d, %d, %d)\n",
                       event.xbutton.button, event.xbutton.state,
                       event.xbutton.x, event.xbutton.y);
      break;
    }

    if (event.type == ClientMessage) {
      sglutDestroy();
      break;
    }
  }
}
