#ifndef __SGL_RENDERBUFFER_H__
#define __SGL_RENDERBUFFER_H__

#include "context.h"

extern void _sgl_init_renderbuffer(struct sgl_renderbuffer* buf, GLint type,
                                   GLboolean allocate);

extern void _sgl_free_renderbuffer(struct sgl_renderbuffer* buf);

#endif /* __SGL_RENDERBUFFER_H__ */
