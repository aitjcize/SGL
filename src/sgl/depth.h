#ifndef __SGL_DPETH_H__
#define __SGL_DPETH_H__

#include "context.h"

extern void _sgl_init_depth(struct sgl_context* ctx);

extern void glClearDepth(GLclampd depth);

extern void glClearDepthf(GLclampf depth);

extern void glDepthFunc(GLenum func);

extern void glDepthMask(GLboolean flag);

#endif /* __SGL_DPETH_H__ */
