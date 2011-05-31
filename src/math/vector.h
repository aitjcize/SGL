/*
 * Vector Math Library
 *
 * Most of code in this file is taken from Mesa
 *
 */

#ifndef __SGL_MATH_VECTOR_H__
#define __SGL_MATH_VECTOR_H__

#include "sglHeaders.h"

#define VEC_MALLOC 0x1

typedef struct _SGLvector4f SGLvector4f;
struct _SGLvector4f {
  GLfloat (*data)[4];     /* point to malloced data or client data */
  GLfloat* start;         /* points somewhere inside of <data> */
  GLuint count;           /* size of the vector (in elements) */
  GLuint stride;          /* stride from one element to the next (in bytes) */
  GLuint size;            /* element size 2-4 */
  void* storage;          /* self-allocated storage */
  GLuint storage_count;   /* storage size in elements */
  GLbitfield flags;       /* VEC_X flags */
};

extern void _sgl_vector4f_init(SGLvector4f *v, GLbitfield flags,
                               GLfloat (*storage)[4]);

extern void _sgl_vector4f_alloc(SGLvector4f *v, GLbitfield flags,
                                GLuint count);

extern void _sgl_vector4f_alloc_double(SGLvector4f *v);

extern void _sgl_vector4f_free(SGLvector4f* v);

extern GLboolean _sgl_vector4f_push_back(SGLvector4f* v, GLfloat* data,
                                         GLuint size);

extern GLboolean _sgl_vector4f_pop_back(SGLvector4f* v, GLfloat* data,
                                        GLuint size);

extern void _sgl_vector4f_print(SGLvector4f* v);

/*
 * Given vector <v>, return <type> pointer of the <i>-th element.
 */
#define VEC_ELT(v, type, i) \
        ((type*) ((GLbyte*) ((v)->data) + (i) * (v)->stride))

#endif /* __SGL_MATH_VECTOR_H__ */
