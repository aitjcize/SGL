/**
 * @file   pipeline.c
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
