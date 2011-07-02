/**
 * @file   polygon.c
 *
 * Copyright (C) 2011 - SGL Authors <aitjcize@gmail.com>
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

#include "polygon.h"

#include "logging.h"

void glPolygonMode(GLenum face, GLenum mode)
{
  GET_CURRENT_CONTEXT(ctx);

  switch (mode) {
  case GL_POINT:
  case GL_LINE:
  case GL_FILL:
    break;
  default:
    _sgl_error(ctx, GL_INVALID_ENUM, "glPolygonMode(): Invalid polygon mode\n");
    return;
  }

  switch (face) {
  case GL_FRONT:
    ctx->polygon.front = mode;
    break;
  case GL_BACK:
    ctx->polygon.back = mode;
    break;
  case GL_FRONT_AND_BACK:
    ctx->polygon.front = ctx->polygon.back = mode;
    break;
  default:
    _sgl_error(ctx, GL_INVALID_ENUM, "glPolygonMode(): Invalid face\n");
  }
}
