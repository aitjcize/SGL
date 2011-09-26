/**
 * @file   framebuffer.c
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

#include "framebuffer.h"

#include <stdlib.h>
#include <string.h>

#include "context.h"
#include "renderbuffer.h"
#include "logging.h"

void _sgl_init_framebuffer(struct sgl_context* ctx)
{
  struct sgl_framebuffer* buf = NULL;
  GLuint et_size = ctx->buffer.height * EDGE_TABLE_SIZE * sizeof(GLint);

  buf = malloc(sizeof(struct sgl_framebuffer));
  buf->width = ctx->buffer.width;
  buf->height = ctx->buffer.height;
  buf->depth_max = 65535;

  _sgl_init_renderbuffer(&buf->clear_color_buf, GL_UNSIGNED_INT, GL_TRUE);
  _sgl_init_renderbuffer(&buf->clear_depth_buf, GL_FLOAT, GL_TRUE);

  _sgl_init_renderbuffer(&buf->color_buf, GL_UNSIGNED_INT, GL_FALSE);
  _sgl_init_renderbuffer(&buf->depth_buf, GL_FLOAT, GL_TRUE);

  /* Clear Edge Table */
  buf->edge_tab = malloc(et_size);
  memset(buf->edge_tab, 0, et_size);

  ctx->drawbuffer = buf;
}

void _sgl_free_framebuffer_data(struct sgl_context* ctx)
{
  _sgl_free_renderbuffer(&ctx->drawbuffer->clear_color_buf);
  _sgl_free_renderbuffer(&ctx->drawbuffer->clear_depth_buf);

  _sgl_free_renderbuffer(&ctx->drawbuffer->color_buf);
  _sgl_free_renderbuffer(&ctx->drawbuffer->depth_buf);
}

void _sgl_clear_framebuffer(GLenum mask)
{
  GET_CURRENT_CONTEXT(ctx);
  GLint size = ctx->buffer.width * ctx->buffer.height;

  if ((mask & GL_COLOR_BUFFER_BIT) == GL_COLOR_BUFFER_BIT)
    memcpy(ctx->drawbuffer->color_buf.data,
           ctx->drawbuffer->clear_color_buf.data,
           size * sizeof(GLuint));

  if ((mask & GL_DEPTH_BUFFER_BIT) == GL_DEPTH_BUFFER_BIT)
    memcpy(ctx->drawbuffer->depth_buf.data,
           ctx->drawbuffer->clear_depth_buf.data,
           size * sizeof(GLfloat));
}

void _sgl_framebuffer_depth(GLboolean status)
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->depth.test = status;
}

void _sgl_framebuffer_edge_table_clear(void)
{
  GET_CURRENT_CONTEXT(ctx);

  memset(ctx->drawbuffer->edge_tab, 0,
         ctx->drawbuffer->height * EDGE_TABLE_SIZE * sizeof(GLint));
}
