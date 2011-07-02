/**
 * @file   context.h
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

#ifndef __SGL_CONTEXT_H__
#define __SGL_CONTEXT_H__

#include "types.h"
#include "logging.h"

#define GL_ENABLED(ctx, type) ((ctx->flags & type) == type)

/*
 * @brief Flush vertices.
 *
 * @param ctx GL context.
 * @param newstate new state.
 *
 * Checks if dd_function_table::NeedFlush is marked to flush stored vertices,
 * and calls dd_function_table::FlushVertices if so. Marks
 * __struct gl_contextRec::NewState with \p newstate.
 */
#define FLUSH_VERTICES(ctx, newstate)				\
do {								\
} while (0)
/*   if (ctx->render_state.needflush & FLUSH_STORED_VERTICES)		\
       ctx->render_state.flushvertices(ctx, FLUSH_STORED_VERTICES);	\*/

/*
 * @brief Macro to assert that the API call was made outside the
 * glBegin()/glEnd() pair and flush the vertices, with return value.
 *
 * @para ctx GL context.
 * @return value to return value in case the assertion fails.
 */
#define ASSERT_OUTSIDE_BEGIN_END_WITH_RETVAL(ctx, retval)                   \
do {                                                                        \
  if (ctx->render_state.current_exec_primitive != PRIM_OUTSIDE_BEGIN_END) { \
    _sgl_error(ctx, GL_INVALID_OPERATION, "Inside glBegin/glEnd");          \
    return retval;                                                          \
  }                                                                         \
} while(0)

/*
 * @brief Macro to assert that the API call was made outside the
 * glBegin()/glEnd() pair.
 *
 * @param ctx GL context.
 */
#define ASSERT_OUTSIDE_BEGIN_END(ctx)                                       \
do {                                                                        \
  if (ctx->render_state.current_exec_primitive != PRIM_OUTSIDE_BEGIN_END) { \
    _sgl_error(ctx, GL_INVALID_OPERATION, "Inside glBegin/glEnd");          \
    return;                                                                 \
  }                                                                         \
} while(0)

/*
 * @brief Macro to assert that the API call was made outside the
 * glBegin()/glEnd() pair and flush the vertices.
 *
 * @param ctx GL context.
 */
#define ASSERT_OUTSIDE_BEGIN_END_AND_FLUSH(ctx)				\
do {									\
   ASSERT_OUTSIDE_BEGIN_END(ctx);					\
   FLUSH_VERTICES(ctx, 0);						\
} while (0)

/*
 * @breif Macro to assert that the API call was made outside the
 * glBegin()/glEnd() pair and flush the vertices, with return value.
 *
 * @param ctx GL context.
 * @param retval value to return value in case the assertion fails.
 */
#define ASSERT_OUTSIDE_BEGIN_END_AND_FLUSH_WITH_RETVAL(ctx, retval)	\
do {									\
   ASSERT_OUTSIDE_BEGIN_END_WITH_RETVAL(ctx, retval);			\
   FLUSH_VERTICES(ctx, 0);						\
} while (0)

extern void _sgl_render_state_edge_table_clear(struct sgl_render_state* rd);

extern void glClear(GLbitfield mask);

extern void glBegin(GLenum mode);
extern void glEnd(void);

void glEnable(GLenum cap);
void glDisable(GLenum cap);

extern void glEnableClientState(GLenum cap);
extern void glDisableClientState(GLenum cap);

#endif /* __SGL_CONTEXT_H__ */
