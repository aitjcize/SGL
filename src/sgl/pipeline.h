#ifndef __SGL_PIPLINE_H__
#define __SGL_PIPLINE_H__

#include "types.h"

extern void _sgl_init_pipeline(struct sgl_context* ctx);

extern void _sgl_pipeline_iteration(void);

extern void _sgl_pipeline_depth_test(void);

#endif /* __SGL_PIPLINE_H__ */
