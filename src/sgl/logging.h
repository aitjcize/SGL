#ifndef __SGL_LOGGING_H__
#define __SGL_LOGGING_H__

#include "types.h"

extern void _sgl_debug(struct sgl_context* ctx, const char* fmtstr, ...);

extern void _sgl_error(struct sgl_context* ctx, GLenum error,
                       const char* fmtstr, ...);

#endif /* __SGL_LOGGING_H__ */
