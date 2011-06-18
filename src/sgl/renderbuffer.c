/**
 * @file   renderbuffer.c
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

#include "renderbuffer.h"

#include <stdlib.h>
#include <string.h>

#include "utils.h"

void _sgl_init_renderbuffer(struct sgl_renderbuffer* buf, GLint type,
                            GLboolean allocate)
{
  GET_CURRENT_CONTEXT(ctx);
  GLuint size = 0;
  buf->width = ctx->buffer.width;
  buf->height = ctx->buffer.height;
  buf->type = type;
  buf->allocated = allocate;

  if (allocate) {
    size = ctx->buffer.width * ctx->buffer.height * _sgl_sizeof_type(type);
    buf->data = malloc(size);
    memset(buf->data, 0, size);
  }
}

void _sgl_free_renderbuffer(struct sgl_renderbuffer* buf)
{
  if (buf->allocated)
    free(buf->data);
}
