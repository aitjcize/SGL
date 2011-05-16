/*
 * Matrix Math Library
 */

#ifndef __SGL_MATH_MATRIX_H__
#define __SGL_MATH_MATRIX_H__

#include "glheaders.h"

enum SGLmatrixtype {
  MATRIX_GENERAL,
  MATRIX_IDENTITY
}

/*
 * @brief matirx structure
 * 
 * m:
 *     0  1  2  3
 *     4  5  6  7
 *     8  9  10 11
 *     12 13 14 15
 */

typedef struct {
  GLfloat *m;
  GLfloat *inv;
  GLbitfield flags;
} SGLmatrix;

/*
 * @brief Initialize a matrix
 *
 * Allocates the matrix and set it to Identity
 */
extern void _sgl_matrix_init(SGLmatrix* m);

/*
 * @brief  free a matrix
 */
extern void _sgl_matrix_free(SGLmatrix* m);

/*
 * @brief Allocate the matrix inverse
 *
 * Allocates the matrix inverse and set it to Identity.
 */
extern void _sgl_matrix_alloc_inv(SGLmatrix* m);

extern void _sgl_matrix_mul_matrix(SGLmatrix* product, const SGLmatrix* a,
                                   const SGLmatrix* b);

extern void _sgl_matrix_mul_floats(SGLmatrix* product, const GLfloat* b);

extern void _sgl_matrix_loadf(SGLmatrix* self, const GLfloat* m);

extern void _sgl_matrix_translate(SGLmatrix* self, const GLfloat* m);

extern void _sgl_matrix_rotate(SGLmatrix* self, GLfloat angle, GLfloat x,
                               GLfloat y, GLfloat z);

extern void _sgl_matrix_scale(SGLmatrix* self, GLfloat x, GLfloat y, GLfloat z);

extern void _sgl_matrix_ortho(SGLmatrix* self,
                              GLfloat left, GLfloat right,
                              GLfloat bottom, GLfloat top,
                              GLfloat nerval, GLfloat farval);

extern void _sgl_matrix_frustum(SGLmatrix* self,
                                GLfloat left, GLfloat right,
                                GLfloat bottom, GLfloat top,
                                GLfloat nerval, GLfloat farval);

extern void _sgl_matrix_set_indentity(SGLmatrix* self);

extern void _sgl_matrix_copy(SGLmatrix* to, const SGLmatrix* from);

extern void _sgl_matrix_print(SGLmatrix* m);


#endif /* __SGL_MATH_MATRIX_H__ */
