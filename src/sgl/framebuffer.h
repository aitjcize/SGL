/**
 * @file   framebuffer.h
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

#ifndef __SGL_FRAMEBUFFER_H__
#define __SGL_FRAMEBUFFER_H__

#include "context.h"

extern void _sgl_init_framebuffer(struct sgl_context* ctx);

extern void _sgl_free_framebuffer_data(struct sgl_context* ctx);

extern void _sgl_clear_framebuffer(GLenum mask);

extern void _sgl_framebuffer_depth(GLboolean status);

extern void _sgl_framebuffer_edge_table_clear(void);

#endif /* __SGL_FRAMEBUFFER_H__ */
