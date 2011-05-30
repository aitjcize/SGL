/*
 * Matrix Math Library
 */

#include "matrix.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "utils.h"

#define D2R(x) ((x) * M_PI / 180.0)
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
  self->flags = 0;

  memcpy(self->m, Identity, 16 * sizeof(GLfloat));
}

void _sgl_matrix_free(SGLmatrix* self)
{
  assert(self->m != NULL);
  free(self->m);
}

void _sgl_matrix_alloc_inv(SGLmatrix* self)
{
  if (!self->inv) {
    self->inv = (GLfloat*) malloc(sizeof(GLfloat) * 16);
    memcpy(self->inv, Identity, 16 * sizeof(GLfloat));
  }
}


#define A(row, col) a[(col<<2) + row]
#define B(row, col) b[(col<<2) + row]
#define P(row, col) product[(col<<2) + row]

static void matmul4(GLfloat* product, const GLfloat* a, const GLfloat* b)
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

static void _sgl_matrix_invert(SGLmatrix* self)
{
  _sgl_matrix_alloc_inv(self);
  float inv[16];
  float* m = self->m;
  float det = 0.0;
  int i;

  inv[0] =   m[5]*m[10]*m[15] - m[5]*m[11]*m[14] - m[9]*m[6]*m[15]
           + m[9]*m[7]*m[14] + m[13]*m[6]*m[11] - m[13]*m[7]*m[10];
  inv[4] =  -m[4]*m[10]*m[15] + m[4]*m[11]*m[14] + m[8]*m[6]*m[15]
           - m[8]*m[7]*m[14] - m[12]*m[6]*m[11] + m[12]*m[7]*m[10];
  inv[8] =   m[4]*m[9]*m[15] - m[4]*m[11]*m[13] - m[8]*m[5]*m[15]
           + m[8]*m[7]*m[13] + m[12]*m[5]*m[11] - m[12]*m[7]*m[9];
  inv[12] = -m[4]*m[9]*m[14] + m[4]*m[10]*m[13] + m[8]*m[5]*m[14]
           - m[8]*m[6]*m[13] - m[12]*m[5]*m[10] + m[12]*m[6]*m[9];
  inv[1] =  -m[1]*m[10]*m[15] + m[1]*m[11]*m[14] + m[9]*m[2]*m[15]
           - m[9]*m[3]*m[14] - m[13]*m[2]*m[11] + m[13]*m[3]*m[10];
  inv[5] =   m[0]*m[10]*m[15] - m[0]*m[11]*m[14] - m[8]*m[2]*m[15]
           + m[8]*m[3]*m[14] + m[12]*m[2]*m[11] - m[12]*m[3]*m[10];
  inv[9] =  -m[0]*m[9]*m[15] + m[0]*m[11]*m[13] + m[8]*m[1]*m[15]
           - m[8]*m[3]*m[13] - m[12]*m[1]*m[11] + m[12]*m[3]*m[9];
  inv[13] =  m[0]*m[9]*m[14] - m[0]*m[10]*m[13] - m[8]*m[1]*m[14]
           + m[8]*m[2]*m[13] + m[12]*m[1]*m[10] - m[12]*m[2]*m[9];
  inv[2] =   m[1]*m[6]*m[15] - m[1]*m[7]*m[14] - m[5]*m[2]*m[15]
           + m[5]*m[3]*m[14] + m[13]*m[2]*m[7] - m[13]*m[3]*m[6];
  inv[6] =  -m[0]*m[6]*m[15] + m[0]*m[7]*m[14] + m[4]*m[2]*m[15]
           - m[4]*m[3]*m[14] - m[12]*m[2]*m[7] + m[12]*m[3]*m[6];
  inv[10] =  m[0]*m[5]*m[15] - m[0]*m[7]*m[13] - m[4]*m[1]*m[15]
           + m[4]*m[3]*m[13] + m[12]*m[1]*m[7] - m[12]*m[3]*m[5];
  inv[14] = -m[0]*m[5]*m[14] + m[0]*m[6]*m[13] + m[4]*m[1]*m[14]
           - m[4]*m[2]*m[13] - m[12]*m[1]*m[6] + m[12]*m[2]*m[5];
  inv[3] =  -m[1]*m[6]*m[11] + m[1]*m[7]*m[10] + m[5]*m[2]*m[11]
           - m[5]*m[3]*m[10] - m[9]*m[2]*m[7] + m[9]*m[3]*m[6];
  inv[7] =   m[0]*m[6]*m[11] - m[0]*m[7]*m[10] - m[4]*m[2]*m[11]
           + m[4]*m[3]*m[10] + m[8]*m[2]*m[7] - m[8]*m[3]*m[6];
  inv[11] = -m[0]*m[5]*m[11] + m[0]*m[7]*m[9] + m[4]*m[1]*m[11]
           - m[4]*m[3]*m[9] - m[8]*m[1]*m[7] + m[8]*m[3]*m[5];
  inv[15] =  m[0]*m[5]*m[10] - m[0]*m[6]*m[9] - m[4]*m[1]*m[10]
           + m[4]*m[2]*m[9] + m[8]*m[1]*m[6] - m[8]*m[2]*m[5];

  det = m[0]*inv[0] + m[1]*inv[4] + m[2]*inv[8] + m[3]*inv[12];
  if (det == 0)
      return GL_FALSE;

  det = 1.0 / det;

  for (i = 0; i < 16; i++)
      self->inv[i] = inv[i] * det;

  return GL_TRUE;
}

void _sgl_matrix_mul_matrix(SGLmatrix* product, const SGLmatrix* a,
                            const SGLmatrix* b)
{
  matmul4(product->m, a->m, b->m);
}

void _sgl_matrix_mul_floats(SGLmatrix* product, const GLfloat* b)
{
  matmul4(product->m, product->m, b);
}

void _sgl_matrix_loadf(SGLmatrix* self, const GLfloat* m)
{
  memcpy(self->m, m, 16 * sizeof(GLfloat));
}

void _sgl_matrix_translate(SGLmatrix* mat, GLfloat x , GLfloat y ,GLfloat z)
{
  GLfloat *m = mat->m;
  m[12] = x * m[0] + y * m[4] + z * m[8] + m[12]; 
  m[13] = x * m[1] + y * m[5] + z * m[9] + m[13];
  m[14] = x * m[2] + y * m[6] + z * m[10] + m[14];
  m[15] = x * m[3] + y * m[7] + z * m[11] + m[15];
}

void _sgl_matrix_rotate(SGLmatrix* self, GLfloat angle, GLfloat x,
                        GLfloat y, GLfloat z)
{
  GLfloat ux, uy, uz, xy, xz, yz, norm, dcos, dsin;
  norm = sqrt(x * x + y * y + z * z);
  ux = x / norm;
  uy = y / norm;
  uz = z / norm;
  xy = ux * uy;
  xz = ux * uz;
  yz = uy * uz;
  dcos = cos(D2R(angle));
  dsin = sin(D2R(angle));

  SGLmatrix rotate;
  _sgl_matrix_init(&rotate);
  GLfloat *R = rotate.m;
  
  R[0] = dcos + ux * ux * (1-dcos);
  R[1] = xy * (1-dcos) + uz * dsin;
  R[2] = xz * (1-dcos) - uy * dsin;
  R[3] = 0;

  R[4] = xy * (1-dcos) + uz * dsin;
  R[5] = dcos + uy * uy * (1-dcos);
  R[6] = yz * (1-dcos) + ux * dsin;
  R[7] = 0;

  R[8] = xz * (1-dcos) - uy * dsin;
  R[9] = yz * (1-dcos) + ux * dsin;
  R[10]= dcos + uz * uz * (1-dcos);
  R[11]= 0;

  R[12]= 0;
  R[13]= 0;
  R[14]= 0;
  R[15]= 1;

  _sgl_matrix_mul_floats(self, rotate.m);
  _sgl_matrix_free(&rotate);
}

void _sgl_matrix_scale(SGLmatrix* self, GLfloat x, GLfloat y, GLfloat z)
{
  GLfloat *m = self->m;
  m[0] *= x ; m[1] *= x ; m[2] *= x; m[3] *= x;
  m[4] *= y ; m[5] *= y ; m[6] *= y; m[7] *= y;
  m[8] *= z ; m[9] *= z ; m[10]*= z; m[11]*= z;
}

void _sgl_matrix_ortho(SGLmatrix* self,
                       GLfloat left, GLfloat right,
                       GLfloat bottom, GLfloat top,
                       GLfloat nerval, GLfloat farval)
{
  SGLmatrix ortho;
  _sgl_matrix_init(&ortho);
  GLfloat *Or = ortho.m; 

  Or[0] = 2.0F / (right - left);
  Or[1] = 0.0F;
  Or[2] = 0.0F;
  Or[3] = 0.0F;

  Or[4] = 0.0F;
  Or[5] = 2.0F / (top - bottom);
  Or[6] = 0.0F;
  Or[7] = 0.0F;

  Or[8] = 0.0F;
  Or[9] = 0.0F;
  Or[10] = -2.0F / (farval - nerval);
  Or[11] = 0.0F;

  Or[12] = -(right + left) / (right - left);
  Or[13] = -(top + bottom) / (top - bottom);
  Or[14] = -(farval + nerval) / (farval - nerval);
  Or[15] = 1.0F;

  sgl_matrix_mul_floats(self, ortho.m);
  _sgl_matrix_free(&ortho);
}

void _sgl_matrix_frustum(SGLmatrix* self,
                         GLfloat left, GLfloat right,
                         GLfloat bottom, GLfloat top,
                         GLfloat nerval, GLfloat farval)
{
  SGLmatrix frustum;
  _sgl_matrix_init(&frustum);
  GLfloat *Fr = frustum.m;

  Fr[0] = (2.0F * nerval) / (right - left);
  Fr[1] = 0.0F;
  Fr[2] = 0.0F;
  Fr[3] = 0.0F;

  Fr[4] = 0.0F;
  Fr[5] = (2.0F * nerval) / (top - bottom);
  Fr[6] = 0.0F;
  Fr[7] = 0.0F;

  Fr[8] = (right + left) / (right - left);
  Fr[9] = (top + bottom) / (top - bottom);
  Fr[10] = -(farval + nerval) / (farval - nerval);
  Fr[11] = -1.0F;

  Fr[12] = 0.0F;
  Fr[13] = 0.0F;
  Fr[14] = -(2.0F * farval * nerval) / (farval - nerval);
  Fr[15] = 0.0F;

  sgl_matrix_mul_floats(self, frustum.m);
  _sgl_matrix_free(&frustum);
}

void _sgl_matrix_set_identity(SGLmatrix* self)
{
  memcpy(self->m, Identity, 16 * sizeof(GLfloat));
}

void _sgl_matrix_copy(SGLmatrix* to, const SGLmatrix* from)
{
  memcpy(to->m, from->m, 16 * sizeof(GLfloat));
  to->flags = from->flags;

  if (to->inv != 0) {
    if (from->inv == 0)
      _sgl_matrix_invert(to);
    else
      memcpy(to->inv, from->inv, 16 * sizeof(GLfloat));
  }
}

void _sgl_matrix_print(SGLmatrix* m)
{
  
}
