#ifndef __SGLUT_SGLUT_H__
#define __SGLUT_SGLUT_H__

#ifdef __cplusplus
extern "C" {
#endif

extern void glutSetRenderApp(void (*func)(char* framebuffer));

extern void glutInit(int* argc, char** argv);

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

#ifdef __cplusplus
}
#endif

#endif /* __SGLUT_SGLUT_H__ */
