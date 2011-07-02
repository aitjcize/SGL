/**
 * @file   framebuffer.c
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

  _sgl_init_renderbuffer(&buf->t_color_buf, GL_UNSIGNED_INT, GL_TRUE);
  _sgl_init_renderbuffer(&buf->t_depth_buf, GL_FLOAT, GL_TRUE);

  _sgl_init_renderbuffer(&buf->color_buf, GL_UNSIGNED_INT, GL_FALSE);
  _sgl_init_renderbuffer(&buf->depth_buf, GL_FLOAT, GL_TRUE);
  _sgl_init_renderbuffer(&buf->normal_buf, GL_UNSIGNED_INT, GL_TRUE);

  /* Clear Edge Table */
  buf->edge_tab = malloc(et_size);
  memset(buf->edge_tab, 0, et_size);

  ctx->drawbuffer = buf;
}

void _sgl_free_framebuffer_data(struct sgl_context* ctx)
{
  _sgl_free_renderbuffer(&ctx->drawbuffer->clear_color_buf);
  _sgl_free_renderbuffer(&ctx->drawbuffer->clear_depth_buf);

  _sgl_free_renderbuffer(&ctx->drawbuffer->t_color_buf);
  _sgl_free_renderbuffer(&ctx->drawbuffer->t_depth_buf);

  _sgl_free_renderbuffer(&ctx->drawbuffer->color_buf);
  _sgl_free_renderbuffer(&ctx->drawbuffer->depth_buf);
  _sgl_free_renderbuffer(&ctx->drawbuffer->normal_buf);
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
  memset(ctx->drawbuffer->t_color_buf.data, 0,
         ctx->drawbuffer->height * ctx->drawbuffer->width * sizeof(GLint));
  memset(ctx->drawbuffer->t_depth_buf.data, 0,
         ctx->drawbuffer->height * ctx->drawbuffer->width * sizeof(GLfloat));
}
