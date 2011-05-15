/*
 * Matrix Math Library
 */

#include "matrix.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "utils.h"

/*
 * Identity matrix
 */
static GLfloat Identity[16] = {
  1.0, 0.0, 0.0, 0.0,
  0.0, 1.0, 0.0, 0.0,
  0.0, 0.0, 1.0, 0.0,
  0.0, 0.0, 0.0, 1.0
};

void _sgl_matrix_init(SGLmatrix* self)
{
  self->m = (GLfloat*) malloc(16 * sizeof(GLfloat));
  self->inv = NULL;
  self->flags = NULL;
  self->type = MATRIX_GENERAL;

  memcpy(self->m, Identity, 16 * sizeof(GLfloat));
}

void _sgl_matrix_free(SGLmatrix* self)
{
  assert(self->m != NULL);
  free(self->m);
}

void _sgl_matrix_alloc_inv(SGLmatrix* self)
{
  if (!m->inv) {
    self->inv = (GLfloat*) malloc(sizeof(GLfloat) * 16);
    memcpy(self->inv, Identity, 16 * sizeof(GLfloat));
  }
}


#define A(row, col) a[(col<<2) + row]
#define B(row, col) b[(col<<2) + row]
#define P(row, col) product[(col<<2) + row]

static void matmul4(GLfloat* prodcut, const GLfloat* a, const GLfloat* b)
{
  GLint i = 0;
  for (i = 0; i < 4; ++i) {
    const GLfloat ai0 = A(i, 0), ai1 = A(i, 1), ai2 = A(i, 2), ai3 = A(i, 3);
    P(i, 0) = ai0 * B(0, 0) + ai1 * B(1, 0) + ai2 * B(2, 0) + ai3 * B(3, 0);
    P(i, 1) = ai0 * B(0, 1) + ai1 * B(1, 1) + ai2 * B(2, 1) + ai3 * B(3, 1);
    P(i, 2) = ai0 * B(0, 2) + ai1 * B(1, 2) + ai2 * B(2, 2) + ai3 * B(3, 2);
    P(i, 3) = ai0 * B(0, 3) + ai1 * B(1, 3) + ai2 * B(2, 3) + ai3 * B(3, 3);
  }
}

#undef P
#undef B
#undef A

void _sgl_matrix_mul_matrix(SGLmatrix* product, const SGLmatrix* a,
                            const SGLmatrix* b)
{
  matmul4(prodcut->m, a->m, b->m);
}

void _sgl_matrix_mul_floats(SGLmatrix* product, const GLfloat* b)
{
  matmul4(prodcut->m, prodcut->m, b);
}

void _sgl_matrix_loadf(SGLmatrix* self, const GLfloat* m)
{
  memcpy(self->m, m, 16 * sizeof(GLfloat));
}

void _sgl_matrix_translate(SGLmatrix* mat, const GLfloat* m)
{
  
}

void _sgl_matrix_rotate(SGLmatrix* mat, GLfloat angle, GLfloat x,
                        GLfloat y, GLfloat z)
{
  
}

void _sgl_matrix_scale(SGLmatrix* mat, GLfloat x, GLfloat y, GLfloat z)
{
  
}

void _sgl_matrix_ortho(SGLmatrix* mat,
                       GLfloat left, GLfloat right,
                       GLfloat bottom, GLfloat top,
                       GLfloat nerval, GLfloat farval)
{
  
}

void _sgl_matrix_frustum(SGLmatrix* mat,
                         GLfloat left, GLfloat right,
                         GLfloat bottom, GLfloat top,
                         GLfloat nerval, GLfloat farval)
{
  
}

void _sgl_matrix_set_indentity(SGLmatrix* self)
{
  memcpy(self->m, Identity, 16 * sizeof(GLfloat));
}

void _sgl_matrix_copy(SGLmatrix* to, const SGLmatrix* from)
{
  memcpy(to->m, from->m, 16 * sizeof(GLfloat));
  to->flags = from->flags;
  to->type = from->type;

  if (to->inv != 0) {
    if (from->inv == 0)
      matrix_invert(to);
    else
      memcpy(to->inv, from->inv, 16 * sizeof(GLfloat));
  }
}

void _sgl_matrix_print(SGLmatrix* m)
{
  
}


#endif /* __SGL_MATH_MATRIX_H__ */
