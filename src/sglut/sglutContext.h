#ifndef __SGLUT_SGLUTCONTEXT_H__
#define __SGLUT_SGLUTCONTEXT_H__

#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>

typedef struct _SGLUTContext SGLUTContext;
struct _SGLUTContext {
  /* XLib Related */
  Display* display;
  Window window;
  GC gc;
  int win_x;
  int win_y;
  int win_width;
  int win_height;

  /* Framebuffer */
  XImage* framebuffer_image[2];
  char* framebuffer[2];
};

void sglutInit(int argc, char* argv[]);
void sglutInitWindowSize(int width, int height);
void sglutInitWindowPosition(int x, int y);
void sglutCreateWindow(char* name);
void sglutBindBuffer(char* buf1, char* buf2);
void sglutMainLoop(void);

#endif /* __SGLUT_SGLUTCONTEXT_H__ */
