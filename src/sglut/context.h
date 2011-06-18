/**
 * @file   context.h
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

extern void glutPassiveMotionFunc(void (*func)(int x, int y));

extern void glutMainLoop(void);

#endif /* __SGLUT_CONTEXT_H__ */
