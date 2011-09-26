/**
 * @file   depth.c
 *
 * This file is taken from the Mesa 3D project.
 * Copyright (C) 1999-2007  Brian Paul   All Rights Reserved.
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

#include "depth.h"

#include <string.h>

#include "context.h"
#include "logging.h"
#include "macros.h"

void _sgl_init_depth(struct sgl_context* ctx)
{
  ctx->depth.test = GL_FALSE;
  ctx->depth.clear = 1.0;
  ctx->depth.func = GL_LESS;
  ctx->depth.mask = GL_TRUE;

  int i = 0;
  for (i = 0; i < ctx->buffer.width * ctx->buffer.height; ++i)
    *((GLfloat*)ctx->drawbuffer->clear_depth_buf.data + i) = ctx->depth.clear;

  memcpy(ctx->drawbuffer->depth_buf.data,
         ctx->drawbuffer->clear_depth_buf.data,
         ctx->buffer.width * ctx->buffer.height * sizeof(GLfloat));
}

void glClearDepth(GLclampd depth)
{
  GET_CURRENT_CONTEXT(ctx);
  ASSERT_OUTSIDE_BEGIN_END(ctx);

  depth = CLAMP(depth, 0.0, 1.0);

  if (ctx->depth.clear == depth)
    return;

  ctx->depth.clear = depth;

  int i = 0;
  for (i = 0; i < ctx->buffer.width * ctx->buffer.height; ++i)
    *((GLfloat*)ctx->drawbuffer->clear_depth_buf.data + i) = ctx->depth.clear;
}

void glClearDepthf(GLclampf depth)
{
  glClearDepth(depth);
}

void glDepthFunc(GLenum func)
{
  GET_CURRENT_CONTEXT(ctx);
  ASSERT_OUTSIDE_BEGIN_END(ctx);

  switch (func) {
  case GL_LESS:    /* (default) pass if incoming z < stored z */
  case GL_GEQUAL:
  case GL_LEQUAL:
  case GL_GREATER:
  case GL_NOTEQUAL:
  case GL_EQUAL:
  case GL_ALWAYS:
  case GL_NEVER:
    break;
  default:
    _sgl_error(ctx, GL_INVALID_ENUM, "glDepth.Func");
    return;
  }

  if (ctx->depth.func == func)
    return;

  ctx->depth.func = func;
}

void glDepthMask(GLboolean flag)
{
  GET_CURRENT_CONTEXT(ctx);
  ASSERT_OUTSIDE_BEGIN_END(ctx);
  ctx->depth.mask = flag;
}
