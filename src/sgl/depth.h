#ifndef __SGL_DEPTH_H__
#define __SGL_DEPTH_H__

#include "types.h"

extern void _sgl_init_depth(struct sgl_context* ctx);

extern void glClearDepth(GLclampd depth);

extern void glDepthFunc(GLenum func);

extern void glDepthMask(GLboolean flag);

#endif /* __SGL_DEPTH_H__ */
