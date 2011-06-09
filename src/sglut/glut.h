#ifndef __SGLUT_SGLUT_H__
#define __SGLUT_SGLUT_H__

#ifdef __cplusplus
extern "C" {
#endif

#define GLUT_DEBUG                          (1 << 31)
#define  GLUT_RGB                           0x0000
#define  GLUT_RGBA                          0x0000
#define  GLUT_SINGLE                        0x0000
#define  GLUT_DOUBLE                        0x0002
#define  GLUT_DEPTH                         0x0010

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

#ifdef __cplusplus
}
#endif

#endif /* __SGLUT_SGLUT_H__ */
