#ifndef __SGL_SGLCONTEXT_H__
#define __SGL_SGLCONTEXT_H__

#include "glHeader.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "utils/list.h"

typedef enum {
  GL_MODELVIEW = 0,
  GL_PROJECTION,
  GL_TEXTURE,
  GL_COLOR
} SGLmatrixMode;

typedef struct _SGLContext SGLContext;
struct _SGLContext {
  /* Matrix */
  SGLmatrixMode matrix_mode;
  SGLmatrix* matrix;
  SList* matrix_stack;

  /* Geometry */
  SGLvector4f* vector_point;
  SGLvector4f* vector_normal;
  SGLvector4f* vector_color;
};

#endif /* __SGL_SGLCONTEXT_H__ */
