/**
 * @file   glut.h
 *
 * Copyright (C) 2011 - SGL Authors <aitjcize@gmail.com>
 * All Rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

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

extern void glutPassiveMotionFunc(void (*func)(int x, int y));

extern void glutMainLoop(void);

extern void glutWireTeapot(GLdouble size);
extern void glutSolidTeapot(GLdouble size);

extern void glutWireCube(GLdouble size);
extern void glutSolidCube(GLdouble size);

extern void glutWireSphere(GLdouble radius, GLint slices, GLint stacks);
extern void glutSolidSphere(GLdouble radius, GLint slices, GLint stacks);

#ifdef __cplusplus
}
#endif

#endif /* __SGLUT_SGLUT_H__ */
