#ifndef __SGLUT_SGLUTCONTEXT_H__
#define __SGLUT_SGLUTCONTEXT_H__

#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>

#define GLUT_ENABLED(flag) ((ctx->flags & (flag)) > 0)

#define GLUT_DEBUG    (1 << 1)
#define GLUT_RGBA     (1 << 2)
#define GLUT_RGB      GLUT_RGBA
#define GLUT_SINGLE   (1 << 3)
#define GLUT_DOUBLE   (1 << 4)
#define GLUT_DEPTH    (1 << 4)

struct SGLUTContext {
  /* XLib Related */
  Display* display;
  Window window;
  GC gc;
  int win_x;
  int win_y;
  int win_width;
  int win_height;

  /* Buffers */
  int buf_index;
  XImage* framebuffer_image[2];
  char* framebuffer[2];
  char* depthbuffer[2];
  void (*renderApp)(char* framebuffer);

  /* Events handler */
  void (*displayFunc)(void);
  void (*mouseFunc)(int button, int state, int x, int y);
  void (*motionFunc)(int x, int y);
  void (*keyboardFunc)(unsigned char key, int x, int y);

  /* Misc */
  int flags;
};

extern void glutSetRenderApp(void (*func)(char* framebuffer));

extern void glutInit(int argc, char* argv[]);

extern void glutInitWindowSize(int width, int height);

extern void glutInitWindowPosition(int x, int y);

extern void glutInitDisplayMode(unsigned int mode);

extern void glutCreateWindow(char* name);

extern void glutSetWindowTitle(char* name);

extern void glutSwapBuffers(void);

extern void glutDisplayFunc(void (*func)(void));

extern void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y));

extern void glutMouseFunc(void (*func)(int button, int state, int x, int y));

extern void glutMotionFunc(void (*func)(int x, int y));

extern void glutMainLoop(void);

#endif /* __SGLUT_SGLUTCONTEXT_H__ */
