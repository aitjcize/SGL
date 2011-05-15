/*
 * Vector Math Library
 *
 * Most of code in this file is taken from Mesa
 *
 */

#ifndef __SGL_MATH_VECTOR_H__
#define __SGL_MATH_VECTOR_H__

#include "glheader.h"

#define VEC_MALLOC 0x1

typedef struct {
  GLfloat (*data)[4];     /* point to malloced data or client data */
  GLfloat* start;         /* points somewhere inside of <data> */
  GLuint count;           /* size of the vector (in elements) */
  GLuint stride;          /* stride from one element to the next (in bytes) */
  GLuint size;            /* element size 2-4 */
  void* storage;          /* self-allocated storage */
  GLbitfield flags;       /* VEC_X flags */
} SGLvector4f;

extern void _sgl_vector4f_init(SGLvector4f *v, GLbitfield flags,
                               GLfloat (*storage)[4]);

extern void _sgl_vector4f_alloc(SGLvector4f *v, GLbitfield flags,
                                GLuint count);

extern void _sgl_vector4f_free(SGLvector4f* v);

extern void _sgl_vector4f_print(SGLvector4f* v);

/*
 * Given vector <v>, return <type> pointer of the <i>-th element.
 */
#define VEC_ELT(v, type, i) \
        ((type*) ((GLbyte*) ((v)->data) + (i) * (v)->stride))

#endif /* __SGL_MATH_VECTOR_H__ */
