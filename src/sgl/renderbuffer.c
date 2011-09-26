/**
 * @file   renderbuffer.c
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
