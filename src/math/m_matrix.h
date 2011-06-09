/*
 * Matrix Math Library
 */

#ifndef __SGL_MATH_MATRIX_H__
#define __SGL_MATH_MATRIX_H__

#include "sglheader.h"

/**
 * @brief Symbolic names to some of the entries in the matrix
 * These are handy for the viewport mapping, which is expressed as a matrix.
 */
#define MAT_SX 0
#define MAT_SY 5
#define MAT_SZ 10
#define MAT_TX 12
#define MAT_TY 13
#define MAT_TZ 14

/*
 * @brief matirx element layout
 * 
 * m:
 *     0  4  8  12
 *     1  5  9  13
 *     2  6  10 14
 *     3  7  11 15
 */

typedef struct _SGLmatrix SGLmatrix;
struct _SGLmatrix
{
  GLfloat *m;
  GLfloat *inv;
  GLbitfield flags;
};

/*
 * @brief Initialize a matrix
 *
 * Allocates the matrix and set it to Identity
 */
extern void _math_matrix_init(SGLmatrix* m);

/*
 * @brief  free a matrix
 */
extern void _math_matrix_free(SGLmatrix* m);

/*
 * @brief Allocate the matrix inverse
 *
 * Allocates the matrix inverse and set it to Identity.
 */
extern void _math_matrix_alloc_inv(SGLmatrix* m);

extern void _math_matrix_mul_matrix(SGLmatrix* product, const SGLmatrix* a,
                                    const SGLmatrix* b);

extern void _math_matrix_mul_floats(SGLmatrix* product, const GLfloat* b);

extern void _math_matrix_loadf(SGLmatrix* self, const GLfloat* m);

extern void _math_matrix_translate(SGLmatrix* mat, GLfloat x, GLfloat y,
                                   GLfloat z);

extern void _math_matrix_rotate(SGLmatrix* self, GLfloat angle, GLfloat x,
                                GLfloat y, GLfloat z);

extern void _math_matrix_scale(SGLmatrix* self, GLfloat x, GLfloat y,
                               GLfloat z);

extern void _math_matrix_ortho(SGLmatrix* self,
                               GLfloat left, GLfloat right,
                               GLfloat bottom, GLfloat top,
                               GLfloat nearval, GLfloat farval);

extern void _math_matrix_frustum(SGLmatrix* self,
                                 GLfloat left, GLfloat right,
                                 GLfloat bottom, GLfloat top,
                                 GLfloat nearval, GLfloat farval);

extern void _math_matrix_viewport(SGLmatrix *m, GLint x, GLint y,
                                  GLint width, GLint height,
                                  GLfloat zNear, GLfloat zFar,
                                  GLfloat depthMax);

extern void _math_matrix_set_identity(SGLmatrix* self);

extern void _math_matrix_copy(SGLmatrix* to, const SGLmatrix* from);

extern void _math_matrix_print(SGLmatrix* mat);

#endif /* __SGL_MATH_MATRIX_H__ */
