/*
 * Vector Math Library
 */

#ifndef __SGL_MATH_VECTOR_H__
#define __SGL_MATH_VECTOR_H__

#include "GL/gl.h"

#define VEC_MALLOC 0x1

typedef struct {
  GLfloat (*data)[4];
  GLuint count; 
  GLuint stride;
  GLuint size;
  GLbitfield flags;
} SGLvector4f;

extern void _sgl_vector4f_init(SGLvector4f *v, GLbitfield flags,
                               GLfloat (*storage)[4]);

extern void _sgl_vector4f_alloc(SGLvector4f *v, GLbitfield flags);

extern void _sgl_vector4f_free(SGLvector4f* v);

extern void _sgl_vector4f_print(SGLvector4f* v);

extern void _sgl_vector4f_clear(SGLvector4f* v);

/*
 * Given vector <v>, return <type> pointer of the <i>-th element.
 */
#define VEC_ELT(v, type, i) \
  ((type*) ((GLbyte*) ((v)->data) + (i) * (v)->stride))

#endif /* __SGL_MATH_VECTOR_H__ */
