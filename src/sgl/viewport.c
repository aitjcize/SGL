/**
 * @file   viewport.c
 *
 * Copyright (C) 2011 - Wei-Ning Huang <aitjcize@gmail.com>
 * All Rights reserved.
 *
 * This file is taken from the Mesa-3D project and partially modified.
 * Copyright (C) 2009  VMware, Inc.  All Rights Reserved.
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

#include "viewport.h"

#include "config.h"
#include "context.h"
#include "macros.h"

void _sgl_init_viewport(struct sgl_context *ctx)
{
  GLfloat depthMax = 65535.0F;

  /* viewport group */
  ctx->viewport.x = 0;
  ctx->viewport.y = 0;
  ctx->viewport.width = 0;
  ctx->viewport.height = 0;
  ctx->viewport.near = 0.0;
  ctx->viewport.far = 1.0;
  _math_matrix_init(&ctx->viewport.window_map);

  _math_matrix_viewport(&ctx->viewport.window_map, 0, 0, 0, 0,
                        0.0F, 1.0F, depthMax);
}

void _sgl_free_viewport_data(struct sgl_context *ctx)
{
  _math_matrix_free(&ctx->viewport.window_map);
}

void _sgl_set_viewport(struct sgl_context *ctx, GLint x, GLint y,
                       GLsizei width, GLsizei height)
{
  /* clamp width and height to the implementation dependent range */
  width  = MIN2(width, MAX_VIEWPORT_WIDTH);
  height = MIN2(height, MAX_VIEWPORT_HEIGHT);

  ctx->viewport.x = x;
  ctx->viewport.width = width;
  ctx->viewport.y = y;
  ctx->viewport.height = height;

  _math_matrix_viewport(&ctx->viewport.window_map,
                        ctx->viewport.x, ctx->viewport.y,
                        ctx->viewport.width, ctx->viewport.height,
                        ctx->viewport.near, ctx->viewport.far,
                        ctx->drawbuffer->depth_max);
}

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
  GET_CURRENT_CONTEXT(ctx);
  ASSERT_OUTSIDE_BEGIN_END_AND_FLUSH(ctx);
  _sgl_set_viewport(ctx, x, y, width, height);
}

void glDepthRange(GLclampd nearval, GLclampd farval)
{
  GET_CURRENT_CONTEXT(ctx);
  ASSERT_OUTSIDE_BEGIN_END_AND_FLUSH(ctx);

  ctx->viewport.near = CLAMP(nearval, 0.0, 1.0);
  ctx->viewport.far = CLAMP(farval, 0.0, 1.0);

  _math_matrix_viewport(&ctx->viewport.window_map,
                        ctx->viewport.x, ctx->viewport.y,
                        ctx->viewport.width, ctx->viewport.height,
                        ctx->viewport.near, ctx->viewport.far,
                        ctx->drawbuffer->depth_max);
}

void glDepthRangef(GLclampf nearval, GLclampf farval)
{
  glDepthRange(nearval, farval);
}
