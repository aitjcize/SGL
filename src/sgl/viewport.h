/**
 * @file   viewport.h
 *
 * Copyright (C) 2011 - SGL Authors <aitjcize@gmail.com>
 * All Rights reserved.
 *
 * This file is taken from the Mesa-3D project and partially modified.
 * Copyright (C) 2009  VMware, Inc.  All Rights Reserved.
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

#ifndef __SGL_VIEWPORT_H__
#define __SGL_VIEWPORT_H__

#include "sglheader.h"
#include "context.h"

extern void _sgl_init_viewport(struct sgl_context *ctx);

extern void _sgl_free_viewport_data(struct sgl_context *ctx);

extern void _sgl_set_viewport(struct sgl_context *ctx, GLint x, GLint y,
                              GLsizei width, GLsizei height);

extern void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);

extern void glDepthRange(GLclampd nearval, GLclampd farval);

extern void glDepthRangef(GLclampf nearval, GLclampf farval);

#endif /* __SGL_VIEWPORT_H__ */
