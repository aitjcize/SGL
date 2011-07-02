/**
 * @file   pipeline.c
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

#include "pipeline.h"

#include <string.h>

#include "macros.h"
#include "rasterize.h"
#include "types.h"

void _sgl_init_pipeline(struct sgl_context* ctx)
{
  ctx->pipeline.rasterize = _sgl_pipeline_rasterize;
}

void _sgl_pipeline_iteration(void)
{
  GET_CURRENT_CONTEXT(ctx);
  if (ctx->pipeline.vertex_shader)
    ctx->pipeline.vertex_shader();
  if (ctx->pipeline.rasterize)
    ctx->pipeline.rasterize();
  if (ctx->pipeline.fragment_shader)
    ctx->pipeline.fragment_shader();
}

void sglPipelineIter(char* buffer)
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->drawbuffer->color_buf.data = buffer;
}

#if 0
void _sgl_pipeline_depth_test(void)
{
  GET_CURRENT_CONTEXT(ctx);
  GLint x = 0, y = 0;
  GLfloat dz = 0.0, nz = 0.0;
  struct sgl_framebuffer* buf = ctx->drawbuffer;
  GLint size = ctx->buffer.width * ctx->buffer.height;

  for (y = 0; y < buf->height; ++y)
    for (x = 0; x < buf->width; ++x) {
      dz = NORMALIZE_Z(ctx, BUF_GET_D(&buf->depth_buf, x, y));
      nz = NORMALIZE_Z(ctx, BUF_GET_D(&buf->t_depth_buf, x, y));

      if (nz < dz) {
        BUF_SET_C(&buf->color_buf, x, y, BUF_GET_C(&buf->t_color_buf, x, y));
        BUF_SET_D(&buf->depth_buf, x, y, BUF_GET_D(&buf->t_depth_buf, x, y));
      }
    }

  _sgl_framebuffer_edge_table_clear();
}
#endif
