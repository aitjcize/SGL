#ifndef __SGLUT_SGLUTCONTEXT_H__
#define __SGLUT_SGLUTCONTEXT_H__

#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>

#define SGLUT_DEBUG  (1 << 1)

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
  int buf_index;

  /* Events handler */
  void (*displayFunc)(void);
  void (*mouseFunc)(int button, int state, int x, int y);
  void (*motionFunc)(int x, int y);
  void (*keyboardFunc)(unsigned char key, int x, int y);

  /* Misc */
  int flags;
};

extern void glutInit(int argc, char* argv[]);

extern void glutInitWindowSize(int width, int height);

extern void glutInitWindowPosition(int x, int y);

extern void glutCreateWindow(char* name);

extern void glutBindBuffer(char* buf1, char* buf2);

extern void glutDisplayFunc(void (*func)(void));

extern void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y));

extern void glutMouseFunc(void (*func)(int button, int state, int x, int y));

extern void glutMotionFunc(void (*func)(int x, int y));

extern void glutSwapBuffers(void);

extern void glutMainLoop(void);

#endif /* __SGLUT_SGLUTCONTEXT_H__ */
