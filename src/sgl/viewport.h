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
