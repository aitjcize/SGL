/**
 * @file   utils.c
 *
 * Copyright (C) 2011 - Wei-Ning Huang <aitjcize@gmail.com>
 * All Rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
