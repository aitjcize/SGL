/**
 * @file   color.c
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

#include "color.h"

#include <string.h>

#include "macros.h"

void _sgl_init_color(struct sgl_context* ctx)
{
  ctx->color.clear = 0;

  int i = 0;
  for (i = 0; i < ctx->buffer.width * ctx->buffer.height; ++i)
    *((GLuint*)ctx->drawbuffer->clear_color_buf.data + i) = ctx->color.clear;
}


void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->color.clear = COLOR_FE(red, green, blue, alpha);

  int i = 0;
  for (i = 0; i < ctx->buffer.width * ctx->buffer.height; ++i)
    *((GLuint*)ctx->drawbuffer->clear_color_buf.data + i) = ctx->color.clear;
}
