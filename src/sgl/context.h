#ifndef __SGL_CONTEXT_H__
#define __SGL_CONTEXT_H__

#include "types.h"
#include "logging.h"

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

void _sgl_context_init(struct sgl_context* sc);

void _sgl_context_free(struct sgl_context* sc);

extern void glClear(GLbitfield mask);

extern void glClearColor(GLclampf red, GLclampf green, GLclampf blue,
                         GLclampf alpha);

extern void glBegin(GLenum mode);
extern void glEnd(void);

extern void glEnableClientState(GLenum cap);
extern void glDisableClientState(GLenum cap);

#endif /* __SGL_CONTEXT_H__ */
