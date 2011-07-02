/**
 * @file   context.c
 *
 * Copyright (C) 2011 - SGL Authors <aitjcize@gmail.com>
 * All Rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "context.h"

#include <stdio.h>
#include <stdlib.h>

#include <GL/gl.h>

#include "sglapi.h"
#include "sglutapi.h"

struct sglut_context _g_sglut_context;

void _glutRenderFromApp(void)
{
  GET_SGLUT_CONTEXT(ctx);
  /* Render to backbuffer */
  ctx->renderApp(ctx->framebuffer[(ctx->buf_index +
                                   GLUT_ENABLED(GLUT_DOUBLE)) % 2]);
  if (ctx->displayFunc)
    ctx->displayFunc();
}

void _glutRenderSingleFrame(void)
{
  GET_SGLUT_CONTEXT(ctx);
  if (GLUT_ENABLED(GLUT_DEBUG))
    fprintf(stderr, "**SGLUT** Rendered\n");
  _glutRenderFromApp();
  XPutImage(ctx->display, ctx->window, ctx->gc,
            ctx->framebuffer_image[ctx->buf_index], 0, 0, 0, 0,
            ctx->win_width, ctx->win_height);
  XFlush(ctx->display);
}

void _glutDestroy(void)
{
  GET_SGLUT_CONTEXT(ctx);
  XFreeGC(ctx->display, ctx->gc);
  XDestroyWindow(ctx->display, ctx->window);
  XCloseDisplay(ctx->display);
}

void glutSetRenderApp(void (*func)(char* framebuffer))
{
  GET_SGLUT_CONTEXT(ctx);
  ctx->renderApp = func;
}

void glutInit(int* argc, char** argv)
{
  GET_SGLUT_CONTEXT(ctx);
  ctx->display = XOpenDisplay(NULL);
  ctx->flags = 0;

  int i = 0;
  for (i = 1; i < *argc; ++i)
    if (strcmp(argv[i], "-d") == 0)
      ctx->flags |= GLUT_DEBUG;

  ctx->displayFunc = NULL;
  ctx->mouseFunc = NULL;
  ctx->motionFunc = NULL;
  ctx->passiveMotionFunc = NULL;
  ctx->keyboardFunc = NULL;
  ctx->redisplay = 0;

  glutSetRenderApp(sglPipelineIter);
}

void glutInitWindowSize(int width, int height)
{
  GET_SGLUT_CONTEXT(ctx);
  int i = 0;
  int screen_number = XDefaultScreen(ctx->display);
  ctx->win_width = width;
  ctx->win_height = height;

  /* Initialize SGL */
  sglInit(width, height);

  for (i = 0; i < 1 + GLUT_ENABLED(GLUT_DOUBLE); ++i) {
    /* Create Framebuffer */
    ctx->framebuffer[i] = malloc(width * height * 4 * sizeof(char));

    ctx->framebuffer_image[i] = XCreateImage(ctx->display,
        XDefaultVisual(ctx->display, screen_number),
        XDefaultDepth(ctx->display, screen_number),
        ZPixmap, 0, ctx->framebuffer[i], width, height, 32, 0);

    if (!ctx->framebuffer_image[i]) {
      fprintf(stderr, "**SGLUT** Can't not create XImage.\n");
      exit(1);
    }

    /* Create Depthbuffer */
    if (GLUT_ENABLED(GLUT_DEPTH))
      ctx->depthbuffer[i] = malloc(width * height * 4 * sizeof(char));
  }
  glViewport(0, 0, width, height);
}

void glutInitWindowPosition(int x, int y)
{
  GET_SGLUT_CONTEXT(ctx);
  ctx->win_x = x;
  ctx->win_y = y;
}

void glutInitDisplayMode(unsigned int mode)
{
  GET_SGLUT_CONTEXT(ctx);
  ctx->flags |= mode;
}

void glutCreateWindow(char* name)
{
  GET_SGLUT_CONTEXT(ctx);
  int blackColor = BlackPixel(ctx->display, DefaultScreen(ctx->display));

  ctx->window = XCreateSimpleWindow(ctx->display,
                                    DefaultRootWindow(ctx->display),
                                    ctx->win_x, ctx->win_y,
                                    ctx->win_width, ctx->win_height,
                                    0, blackColor, blackColor);

  Atom wmDeleteMessage = XInternAtom(ctx->display, "WM_DELETE_WINDOW",False);
  XSetWMProtocols(ctx->display, ctx->window, &wmDeleteMessage, 1);

  /* Select Input Mask */
  XSelectInput(ctx->display, ctx->window,
               StructureNotifyMask |
               ExposureMask |
               KeyPressMask |
               ButtonPressMask |
               ButtonMotionMask |
               PointerMotionMask);

  XMapWindow(ctx->display, ctx->window);
  XStoreName(ctx->display, ctx->window, name);
  ctx->gc = XCreateGC(ctx->display, ctx->window, 0, NULL);
}

void glutSetWindowTitle(char* name)
{
  GET_SGLUT_CONTEXT(ctx);
  XStoreName(ctx->display, ctx->window, name);
}

void glutSwapBuffers(void)
{
  GET_SGLUT_CONTEXT(ctx);
  if (GLUT_ENABLED(GLUT_DOUBLE)) {
    ctx->buf_index++;
    ctx->buf_index %= 2;
  }
}

void glutPostRedisplay(void)
{
  GET_SGLUT_CONTEXT(ctx);
  ctx->redisplay = 1;
}

void glutDisplayFunc(void (*func)(void))
{
  GET_SGLUT_CONTEXT(ctx);
  ctx->displayFunc = func;
}

void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y))
{
  GET_SGLUT_CONTEXT(ctx);
  ctx->keyboardFunc = func;
}

void glutMouseFunc(void (*func)(int button, int state, int x, int y))
{
  GET_SGLUT_CONTEXT(ctx);
  ctx->mouseFunc = func;
}

void glutMotionFunc(void (*func)(int x, int y))
{
  GET_SGLUT_CONTEXT(ctx);
  ctx->motionFunc = func;
}

void glutPassiveMotionFunc(void (*func)(int x, int y))
{
  GET_SGLUT_CONTEXT(ctx);
  ctx->passiveMotionFunc = func;
}

void glutMainLoop(void)
{
  GET_SGLUT_CONTEXT(ctx);
  XEvent event;
  while (1) {
    /* Check flags */
    if (ctx->redisplay) {
      _glutRenderSingleFrame();
      ctx->redisplay = 0;
    }
    XNextEvent(ctx->display, &event);
    switch (event.type) {
    case Expose:
      _glutRenderSingleFrame();
      if (GLUT_ENABLED(GLUT_DEBUG))
        fprintf(stderr, "**SGLUT** ExposeEvent\n");
      break;

    case KeyPress:
      if (ctx->keyboardFunc) {
        ctx->keyboardFunc(event.xkey.keycode, event.xkey.x, event.xkey.y);
        if (GLUT_ENABLED(GLUT_DEBUG))
          fprintf(stderr, "**SGLUT** KeyPressEvent: (%d, %d, %d)\n",
              event.xkey.keycode, event.xkey.x, event.xkey.y);
      }
      break;

    case ButtonPress:
      if (ctx->mouseFunc) {
        ctx->mouseFunc(event.xbutton.button, event.xbutton.state,
            event.xbutton.x, event.xbutton.y);
        if (GLUT_ENABLED(GLUT_DEBUG))
          fprintf(stderr, "**SGLUT** ButtonPressEvent: (%d, %d, %d, %d)\n",
              event.xbutton.button, event.xbutton.state,
              event.xbutton.x, event.xbutton.y);
      }
      break;

    case MotionNotify:
      if (event.xmotion.state) {
        if (ctx->motionFunc) {
          ctx->motionFunc(event.xbutton.x, event.xbutton.y);
          if (GLUT_ENABLED(GLUT_DEBUG))
            fprintf(stderr, "**SGLUT** MotionNotifyEvent: (%d, %d)\n",
                event.xbutton.x, event.xbutton.y);
        }
      } else {
        if (ctx->passiveMotionFunc) {
          ctx->passiveMotionFunc(event.xbutton.x, event.xbutton.y);
          if (GLUT_ENABLED(GLUT_DEBUG))
            fprintf(stderr, "**SGLUT** MotionNotifyEvent(Passive): (%d, %d)\n",
                event.xbutton.x, event.xbutton.y);
        }
      }
      break;
    }

    if (event.type == ClientMessage) {
      _glutDestroy();
      break;
    }
  }
}
