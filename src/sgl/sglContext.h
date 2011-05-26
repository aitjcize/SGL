#ifndef __SGL_SGLCONTEXT_H__
#define __SGL_SGLCONTEXT_H__

#include "glHeaders.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "utils/list.h"

typedef struct _SGLContext SGLContext;
struct _SGLContext {
  /* Matrix */
  GLint matrix_mode;
  SGLmatrix* matrix;
  SList* matrix_stack;

  /* Geometry */
  SGLvector4f* vector_point;
  SGLvector4f* vector_normal;
  SGLvector4f* vector_color;
};

#endif /* __SGL_SGLCONTEXT_H__ */
