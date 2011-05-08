/*
 * Matrix Math Library
 */

#ifndef __SGL_MATH_MATRIX_H__
#define __SGL_MATH_MATRIX_H__

#define VEC_MALLOC 0x1

enum SGLmatrixtype {
  MATRIX_GENERAL,
  MATRIX_IDENTITY
}

typedef struct {
  GLfloat *m;
  GLfloat *inv;
  GLuint flags;
} SGLmatrix;

extern void _sgl_matrix_init(SGLmatrix* m);

extern void _sgl_matrix_free(SGLmatrix* m);

extern void _sgl_matrix_alloc_inv(SGLmatrix* m);

extern void _sgl_matrix_mul_matrix(SGLmatrix* dest, const SGLmatrix* a,
                                   const SGLmatrix* b);

extern void _sgl_matrix_mul_float(SGLmatrix* dest, const GLfloat* b);

extern void _sgl_matrix_loadf(SGLmatrix* mat, const GLfloat* m);

extern void _sgl_matrix_translate(SGLmatrix* mat, const GLfloat* m);

extern void _sgl_matrix_rotate(SGLmatrix* mat, GLfloat angle, GLfloat x,
                               GLfloat y, GLfloat z);

extern void _sgl_matrix_scale(SGLmatrix* mat, GLfloat x, GLfloat y, GLfloat z);

extern void _sgl_matrix_ortho(SGLmatrix* mat,
                              GLfloat left, GLfloat right,
                              GLfloat bottom, GLfloat top,
                              GLfloat nerval, GLfloat farval);

extern void _sgl_matrix_frustum(SGLmatrix* mat,
                                GLfloat left, GLfloat right,
                                GLfloat bottom, GLfloat top,
                                GLfloat nerval, GLfloat farval);

extern void _sgl_matrix_set_indentity(SGLmatrix* dest);

extern void _sgl_matrix_copy(SGLmatrix* to, const SGLmatrix* from);

extern void _sgl_matrix_print(SGLmatrix* m);


#endif /* __SGL_MATH_MATRIX_H__ */
