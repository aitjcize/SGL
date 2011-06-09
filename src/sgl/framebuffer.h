#ifndef __SGL_FRAMEBUFFER_H__
#define __SGL_FRAMEBUFFER_H__

#include "context.h"

extern void _sgl_init_framebuffer(struct sgl_context* ctx);

extern void _sgl_free_framebuffer_data(struct sgl_context* ctx);

extern void _sgl_clear_framebuffer(GLenum mask);

#endif /* __SGL_FRAMEBUFFER_H__ */
