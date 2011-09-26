/**
 * @file   sgl/context.c
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

#include "context.h"

#include "color.h"
#include "depth.h"
#include "framebuffer.h"
#include "macros.h"
#include "matrix.h"
#include "pipeline.h"
#include "viewport.h"

#include <stdlib.h>
#include <string.h>

/**
 * Global SGL context
 */
struct sgl_context _g_sgl_context;

void _sgl_init_vector(struct sgl_context* ctx)
{
  _math_vector4f_alloc(&ctx->vector_point, 0, VECTOR_INIT_LEN);
  _math_vector4f_alloc(&ctx->vector_normal, 0, VECTOR_INIT_LEN);
  _math_vector4f_alloc(&ctx->vector_color, 0, VECTOR_INIT_LEN);
  _math_vector4f_alloc(&ctx->flood_fill, 0, VECTOR_INIT_LEN * 100);
}

void _sgl_init_misc_attrib(struct sgl_context* ctx, int w, int h)
{
  ctx->matrix_mode = GL_MODELVIEW;
  ctx->polygon.front = ctx->polygon.back = GL_FILL;
  ctx->buffer.width = w;
  ctx->buffer.height = h;
}

void _sgl_init_render_state(struct sgl_context* ctx)
{
  ctx->render_state.current_exec_primitive = PRIM_OUTSIDE_BEGIN_END;
}

void _sgl_context_init(struct sgl_context* ctx, int w, int h)
{
  _sgl_init_misc_attrib(ctx, w, h);
  _sgl_init_render_state(ctx);
  _sgl_init_vector(ctx);
  _sgl_init_matrix(ctx);
  _sgl_init_viewport(ctx);
  _sgl_init_framebuffer(ctx);
  _sgl_init_color(ctx);
  _sgl_init_depth(ctx);
  _sgl_init_pipeline(ctx);

  ctx->flags = 0;
  ctx->clientstate_flags = 0;
}

void _sgl_context_free(struct sgl_context* ctx)
{
  _sgl_free_matrix_data(ctx);
  _sgl_free_framebuffer_data(ctx);
}

void sglInit(int w, int h) {
  GET_CURRENT_CONTEXT(ctx);
  _sgl_context_init(ctx, w, h);
}

void glClear(GLbitfield mask)
{
  GET_CURRENT_CONTEXT(ctx);
  _sgl_clear_framebuffer(mask);
  _math_vector4f_lazy_free(&ctx->vector_point);
  _math_vector4f_lazy_free(&ctx->vector_normal);
  _math_vector4f_lazy_free(&ctx->vector_color);
}

void glEnable(GLenum cap)
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->flags |= cap;

  switch (cap) {
  case GL_DEPTH_TEST:
    _sgl_framebuffer_depth(GL_TRUE);
  break;
  }
}

void glDisable(GLenum cap)
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->flags |= cap;

  switch (cap) {
  case GL_DEPTH_TEST:
    _sgl_framebuffer_depth(GL_FALSE);
  break;
  }
}

void glEnableClientState(GLenum cap)
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->clientstate_flags |= cap;
}

void glDisableClientState(GLenum cap)
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->clientstate_flags &= ~cap;
}
