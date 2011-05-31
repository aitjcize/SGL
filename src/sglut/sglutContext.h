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

  /* Events handler */
  void (*displayFunc)(void);
  void (*mouseFunc)(int button, int state, int x, int y);
  void (*keyboardFunc)(unsigned char key, int x, int y);
};

void sglutInit(int argc, char* argv[]);

void sglutInitWindowSize(int width, int height);

void sglutInitWindowPosition(int x, int y);

void sglutCreateWindow(char* name);

void sglutBindBuffer(char* buf1, char* buf2);

void sglutDisplayFunc(void (*func)(void));

void sglutKeyboardFunc(void (*func)(unsigned char key, int x, int y));

void sglutMouseFunc(void (*func)(int button, int state, int x, int y));

void sglutMainLoop(void);

#endif /* __SGLUT_SGLUTCONTEXT_H__ */
