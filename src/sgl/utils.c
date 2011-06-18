/**
 * @file   utils.c
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

#include "utils.h"

GLint _sgl_sizeof_type(GLenum type)
{
  switch (type) {
  case GL_BITMAP:
    return 0;
  case GL_UNSIGNED_BYTE:
    return sizeof(GLubyte);
  case GL_BYTE:
    return sizeof(GLbyte);
  case GL_UNSIGNED_SHORT:
    return sizeof(GLushort);
  case GL_SHORT:
    return sizeof(GLshort);
  case GL_UNSIGNED_INT:
    return sizeof(GLuint);
  case GL_INT:
    return sizeof(GLint);
  case GL_FLOAT:
    return sizeof(GLfloat);
  case GL_DOUBLE:
    return sizeof(GLdouble);
  case GL_HALF_FLOAT_ARB:
    return sizeof(GLhalfARB);
  default:
    return -1;
  }
}
