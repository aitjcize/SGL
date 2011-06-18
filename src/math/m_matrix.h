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

extern void _sgl_affine_transform(SGLmatrix* mat, GLfloat* vec);

extern void _math_matrix_copy(SGLmatrix* to, const SGLmatrix* from);

extern void _math_matrix_print(SGLmatrix* mat);

/*
 * Transform a point (column vector) by a matrix:   Q = M * P
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


/*
 * Transform a normal (row vector) by a matrix:  [NX NY NZ] = N * MAT
 */
#define TRANSFORM_NORMAL( TO, N, MAT )				\
do {								\
   TO[0] = N[0] * MAT[0] + N[1] * MAT[1] + N[2] * MAT[2];	\
   TO[1] = N[0] * MAT[4] + N[1] * MAT[5] + N[2] * MAT[6];	\
   TO[2] = N[0] * MAT[8] + N[1] * MAT[9] + N[2] * MAT[10];	\
} while (0)


/**
 * Transform a direction by a matrix.
 */
#define TRANSFORM_DIRECTION( TO, DIR, MAT )			\
do {								\
   TO[0] = DIR[0] * MAT[0] + DIR[1] * MAT[4] + DIR[2] * MAT[8];	\
   TO[1] = DIR[0] * MAT[1] + DIR[1] * MAT[5] + DIR[2] * MAT[9];	\
   TO[2] = DIR[0] * MAT[2] + DIR[1] * MAT[6] + DIR[2] * MAT[10];\
} while (0)

#endif /* __SGL_MATH_MATRIX_H__ */
