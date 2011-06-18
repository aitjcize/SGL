/**
 * @file   types.h
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

#ifndef __SGLUT_TYPES_H__
#define __SGLUT_TYPES_H__

#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>

#define GLUT_ENABLED(flag) ((ctx->flags & (flag)) > 0)

#define GLUT_DEBUG                          (1 << 31)
#define GLUT_RGB                           0x0000
#define GLUT_RGBA                          0x0000
#define GLUT_SINGLE                        0x0000
#define GLUT_DOUBLE                        0x0002
#define GLUT_DEPTH                         0x0010

struct sglut_context
{
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
  void (*passiveMotionFunc)(int x, int y);
  void (*keyboardFunc)(unsigned char key, int x, int y);

  /* Misc */
  int flags;
  int redisplay;
};

#endif /* __SGLUT_TYPES_H__ */
