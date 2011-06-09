#ifndef __SGL_RASTERIZE_H__
#define __SGL_RASTERIZE_H__

#include "types.h"

extern void _sgl_primitive_assembly(void);

extern void _sgl_pipeline_rasterize(void);

extern void glPolygonMode(GLenum face, GLenum mode);

#endif /* __SGL_RASTERIZE_H__ */
