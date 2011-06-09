#ifndef __SGLUT_CONTEXT_H__
#define __SGLUT_CONTEXT_H__

#include "types.h"

extern void glutSetRenderApp(void (*func)(char* framebuffer));

extern void glutInit(int* argc, char** argv);

extern void glutInitWindowSize(int width, int height);

extern void glutInitWindowPosition(int x, int y);

extern void glutInitDisplayMode(unsigned int mode);

extern void glutCreateWindow(char* name);

extern void glutSetWindowTitle(char* name);

extern void glutSwapBuffers(void);

extern void glutPostRedisplay(void);

extern void glutDisplayFunc(void (*func)(void));

extern void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y));

extern void glutMouseFunc(void (*func)(int button, int state, int x, int y));

extern void glutMotionFunc(void (*func)(int x, int y));

extern void glutMainLoop(void);

#endif /* __SGLUT_CONTEXT_H__ */
