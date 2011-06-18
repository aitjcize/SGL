/**
 * @file   viewport.c
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
