#include "context.h"

#include <stdio.h>
#include <stdlib.h>

#include "sglut_api.h"

struct sglut_context _g_sglut_context;

void _glutRenderFromApp(void)
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->renderApp(ctx->framebuffer[ctx->buf_index]);
}

void _glutRenderSingleFrame(void)
{
  GET_CURRENT_CONTEXT(ctx);
  _glutRenderFromApp();
  XPutImage(ctx->display, ctx->window, ctx->gc,
            ctx->framebuffer_image[ctx->buf_index], 0, 0, 0, 0,
            ctx->win_width, ctx->win_height);
  XFlush(ctx->display);
}

void _glutDestroy(void)
{
  GET_CURRENT_CONTEXT(ctx);
  XFreeGC(ctx->display, ctx->gc);
  XDestroyWindow(ctx->display, ctx->window);
  XCloseDisplay(ctx->display);
}

void glutSetRenderApp(void (*func)(char* framebuffer))
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->renderApp = func;
}

void glutInit(int* argc, char** argv)
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->display = XOpenDisplay(NULL);
  ctx->flags = 0;

  int i = 0;
  for (i = 1; i < *argc; ++i)
    if (strcmp(argv[i], "-d") == 0)
      ctx->flags |= GLUT_DEBUG;

  ctx->displayFunc = NULL;
  ctx->mouseFunc = NULL;
  ctx->motionFunc = NULL;
  ctx->keyboardFunc = NULL;
}

void glutInitWindowSize(int width, int height)
{
  GET_CURRENT_CONTEXT(ctx);
  int i = 0;
  int screen_number = XDefaultScreen(ctx->display);
  ctx->win_width = width;
  ctx->win_height = height;

  for (i = 0; i < 1 + GLUT_ENABLED(GLUT_DOUBLE); ++i) {
    /* Create Framebuffer */
    ctx->framebuffer[i] = malloc(width * height * 4 * sizeof(char));

    ctx->framebuffer_image[i] = XCreateImage(ctx->display,
        XDefaultVisual(ctx->display, screen_number),
        XDefaultDepth(ctx->display, screen_number),
        ZPixmap, 0, ctx->framebuffer[i], width, height, 32, 0);

    if (!ctx->framebuffer_image[i]) {
      fprintf(stderr, "Can't not create XImage.\n");
      exit(1);
    }

    /* Create Depthbuffer */
    if (GLUT_ENABLED(GLUT_DEPTH))
      ctx->depthbuffer[i] = malloc(width * height * 4 * sizeof(char));
  }
}

void glutInitWindowPosition(int x, int y)
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->win_x = x;
  ctx->win_y = y;
}

void glutInitDisplayMode(unsigned int mode)
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->flags |= mode;
}

void glutCreateWindow(char* name)
{
  GET_CURRENT_CONTEXT(ctx);
  int blackColor = BlackPixel(ctx->display, DefaultScreen(ctx->display));

  ctx->window = XCreateSimpleWindow(ctx->display,
                                       DefaultRootWindow(ctx->display),
                                       ctx->win_x, ctx->win_y,
                                       ctx->win_width, ctx->win_height,
                                       0, blackColor, blackColor);

  // register interest in the delete window message
  Atom wmDeleteMessage = XInternAtom(ctx->display, "WM_DELETE_WINDOW",False);
  XSetWMProtocols(ctx->display, ctx->window, &wmDeleteMessage, 1);

  /* Select Input Mask */
  XSelectInput(ctx->display, ctx->window,
               StructureNotifyMask |
               ExposureMask |
               KeyPressMask |
               ButtonPressMask |
               ButtonMotionMask);

  XMapWindow(ctx->display, ctx->window);
  XStoreName(ctx->display, ctx->window, name);
  ctx->gc = XCreateGC(ctx->display, ctx->window, 0, NULL);
}

void glutSetWindowTitle(char* name)
{
  GET_CURRENT_CONTEXT(ctx);
  XStoreName(ctx->display, ctx->window, name);
}

void glutSwapBuffers(void)
{
  GET_CURRENT_CONTEXT(ctx);
  if (GLUT_ENABLED(GLUT_DOUBLE)) {
    ctx->buf_index++;
    ctx->buf_index %= 2;
  }
  _glutRenderSingleFrame();
}

void glutDisplayFunc(void (*func)(void))
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->displayFunc = func;
}

void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y))
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->keyboardFunc = func;
}

void glutMouseFunc(void (*func)(int button, int state, int x, int y))
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->mouseFunc = func;
}

void glutMotionFunc(void (*func)(int x, int y))
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->motionFunc = func;
}

void glutMainLoop(void)
{
  GET_CURRENT_CONTEXT(ctx);
  XEvent event;
  while (1) {
    XNextEvent(ctx->display, &event);
    _glutRenderSingleFrame();
    switch (event.type) {
    case Expose:
      if (ctx->displayFunc)
        ctx->displayFunc();
      if (GLUT_ENABLED(GLUT_DEBUG))
        fprintf(stderr, "ExposeEvent\n");
      break;

    case KeyPress:
      if (ctx->keyboardFunc)
        ctx->keyboardFunc(event.xkey.keycode, event.xkey.x, event.xkey.y);
      if (GLUT_ENABLED(GLUT_DEBUG))
        fprintf(stderr, "KeyPressEvent: (%d, %d, %d)\n",
                event.xkey.keycode, event.xkey.x, event.xkey.y);
      break;

    case ButtonPress:
      if (ctx->keyboardFunc)
        ctx->mouseFunc(event.xbutton.button, event.xbutton.state,
                          event.xbutton.x, event.xbutton.y);
      if (GLUT_ENABLED(GLUT_DEBUG))
        fprintf(stderr, "ButtonPressEvent: (%d, %d, %d, %d)\n",
                event.xbutton.button, event.xbutton.state,
                event.xbutton.x, event.xbutton.y);
      break;

    case MotionNotify:
      if (ctx->motionFunc)
        ctx->motionFunc(event.xbutton.x, event.xbutton.y);
      if (GLUT_ENABLED(GLUT_DEBUG))
        fprintf(stderr, "MotionNotifyEvent: (%d, %d)\n",
                event.xbutton.x, event.xbutton.y);
      break;
    }

    if (event.type == ClientMessage) {
      _glutDestroy();
      break;
    }
  }
}
