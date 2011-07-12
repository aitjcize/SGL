/**
 * @file m_matrix.h
 *
 * Matrix Math Library
 *
 * Copyright (C) 2011 - SGL Authors <aitjcize@gmail.com>
 * All Rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Most of this file is taken from the Mesa project, please refer to Mesa
 * library for more information.
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

typedef struct _SGLmatrix SGLmatrix;

/**
 * @brief SGLmatrix structure
 * @struct _SGLmatrix
 *
 * <pre>
 * Cell array layout as follows
 *  0  4  8  12
 *  1  5  9  13
 *  2  6  10 14
 *  3  7  11 15
 * </pre>
 */
struct _SGLmatrix
{
  GLfloat *m;             /**< cell array */
  GLfloat *inv;           /**< pointer to inverse matrix cell array*/
  GLbitfield flags;       /**< flags */
};

/**
 * @brief Initialize a matrix
 *
 * Allocates the matrix and set it to Identity
 */
extern void _math_matrix_init(SGLmatrix* m);

/**
 * @brief  free a matrix
 */
extern void _math_matrix_free(SGLmatrix* m);

/**
 * @brief Allocate the matrix inverse
 * @param m a SGLmatrix
 *
 * Allocates the matrix inverse and set it to Identity.
 */
extern void _math_matrix_alloc_inv(SGLmatrix* m);

/**
 * @brief Multiply two matrices
 * @param product pointer to a matrix which result is stored to
 * @param a left-hand-side matrix
 * @param b right-hand-side matrix
 */
extern void _math_matrix_mul_matrix(SGLmatrix* product, const SGLmatrix* a,
                                    const SGLmatrix* b);

/**
 * @brief Multiply a matrix by a float array inplace
 * @param product source matrix
 * @param b right-hand-side matrix
 */
extern void _math_matrix_mul_floats(SGLmatrix* product, const GLfloat* b);

/**
 * @brief Replace matrix with an array of floats
 * @param self target matrix
 * @param m target float array that you want to set the matrix to
 */
extern void _math_matrix_loadf(SGLmatrix* self, const GLfloat* m);

/**
 * @brief Post-multiply a matrix with a translation matrix
 * @param mat target matrix
 * @param x x-axis offset
 * @param y y-axis offset
 * @param z z-axis offset
 */
extern void _math_matrix_translate(SGLmatrix* mat, GLfloat x, GLfloat y,
                                   GLfloat z);

/**
 * @brief Post-multiply a matrix with a rotation matrix
 * @param mat target matrix
 * @param angle angle to rotate
 * @param x x componment of the axis vector
 * @param y y componment of the axis vector
 * @param z z componment of the axis vector
 */
extern void _math_matrix_rotate(SGLmatrix* self, GLfloat angle, GLfloat x,
                                GLfloat y, GLfloat z);

/**
 * @brief Post-multiply a matrix with a scale matrix
 * @param mat target matrix
 * @param x x-axis scale ratio
 * @param y y-axis scale ratio
 * @param z z-axis scale ratio
 */
extern void _math_matrix_scale(SGLmatrix* self, GLfloat x, GLfloat y,
                               GLfloat z);

/**
 * @brief Apply an orthographic projection matrix.
 * @param mat matrix to apply the projection.
 * @param left left clipping plane coordinate.
 * @param right right clipping plane coordinate.
 * @param bottom bottom clipping plane coordinate.
 * @param top top clipping plane coordinate.
 * @param nearval distance to the near clipping plane.
 * @param farval distance to the far clipping plane.
 */
extern void _math_matrix_ortho(SGLmatrix* self,
                               GLfloat left, GLfloat right,
                               GLfloat bottom, GLfloat top,
                               GLfloat nearval, GLfloat farval);

/**
 * @brief Multiply a matrix with a general scaling matrix.
 * *param mat matrix.
 * *param x x axis scale factor.
 * *param y y axis scale factor.
 * *param z z axis scale factor.
 */
extern void _math_matrix_frustum(SGLmatrix* self,
                                 GLfloat left, GLfloat right,
                                 GLfloat bottom, GLfloat top,
                                 GLfloat nearval, GLfloat farval);

/**
 * Set matrix to do viewport and depthrange mapping.
 * Transforms Normalized Device Coords to window/Z values.
 */
extern void _math_matrix_viewport(SGLmatrix *m, GLint x, GLint y,
                                  GLint width, GLint height,
                                  GLfloat zNear, GLfloat zFar,
                                  GLfloat depthMax);

/**
 * set the target matrix to identity matrix
 */
extern void _math_matrix_set_identity(SGLmatrix* self);

/**
 * @brief Perform affine trnasformation
 * @param mat target matrix
 * @param vec point to be transform
 */
extern void _sgl_affine_transform(SGLmatrix* mat, GLfloat* vec);

/**
 * @brief Copy matrices
 * @param to destination matrix
 * @param from source matrix
 */
extern void _math_matrix_copy(SGLmatrix* to, const SGLmatrix* from);

/**
 * Print a matrix. For debugging.
 */
extern void _math_matrix_print(SGLmatrix* mat);

/**
 * @brief Transform a point (column vector) by a matrix:   Q = M * P
 */
#define TRANSFORM_POINT( Q, M, P )					\
   Q[0] = M[0] * P[0] + M[4] * P[1] + M[8] *  P[2] + M[12] * P[3];	\
   Q[1] = M[1] * P[0] + M[5] * P[1] + M[9] *  P[2] + M[13] * P[3];	\
   Q[2] = M[2] * P[0] + M[6] * P[1] + M[10] * P[2] + M[14] * P[3];	\
   Q[3] = M[3] * P[0] + M[7] * P[1] + M[11] * P[2] + M[15] * P[3];


#define TRANSFORM_POINT3( Q, M, P )				\
   Q[0] = M[0] * P[0] + M[4] * P[1] + M[8] *  P[2] + M[12];	\
   Q[1] = M[1] * P[0] + M[5] * P[1] + M[9] *  P[2] + M[13];	\
   Q[2] = M[2] * P[0] + M[6] * P[1] + M[10] * P[2] + M[14];	\
   Q[3] = M[3] * P[0] + M[7] * P[1] + M[11] * P[2] + M[15];


/**
 * @brief Transform a normal (row vector) by a matrix:  [NX NY NZ] = N * MAT
 */
#define TRANSFORM_NORMAL( TO, N, MAT )				\
do {								\
   TO[0] = N[0] * MAT[0] + N[1] * MAT[1] + N[2] * MAT[2];	\
   TO[1] = N[0] * MAT[4] + N[1] * MAT[5] + N[2] * MAT[6];	\
   TO[2] = N[0] * MAT[8] + N[1] * MAT[9] + N[2] * MAT[10];	\
} while (0)


/**
 * @brief Transform a direction by a matrix.
 */
#define TRANSFORM_DIRECTION( TO, DIR, MAT )			\
do {								\
   TO[0] = DIR[0] * MAT[0] + DIR[1] * MAT[4] + DIR[2] * MAT[8];	\
   TO[1] = DIR[0] * MAT[1] + DIR[1] * MAT[5] + DIR[2] * MAT[9];	\
   TO[2] = DIR[0] * MAT[2] + DIR[1] * MAT[6] + DIR[2] * MAT[10];\
} while (0)

#endif /* __SGL_MATH_MATRIX_H__ */
