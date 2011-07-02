/**
 * @file   config.h
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

#ifndef __SGL_CONFIG_H__
#define __SGL_CONFIG_H__

/* Vector buffer initial length */
#define VECTOR_INIT_LEN 512

/* Maximum modelview matrix stack depth */
#define MAX_MODELVIEW_STACK_DEPTH 32

/* Maximum projection matrix stack depth */
#define MAX_PROJECTION_STACK_DEPTH 32

/* Maximum viewport width and height */
#define MAX_VIEWPORT_WIDTH 1366
#define MAX_VIEWPORT_HEIGHT 768

/* Edge Table size */
#define EDGE_TABLE_SIZE 6

#endif /* __SGL_CONFIG_H__ */
