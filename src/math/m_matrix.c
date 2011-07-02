/*
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

#include "m_matrix.h"

#include <stdio.h>
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

void _math_matrix_init(SGLmatrix* mat)
{
  mat->m = (GLfloat*) malloc(16 * sizeof(GLfloat));
  mat->inv = NULL;
  mat->flags = 0;

  memcpy(mat->m, Identity, 16 * sizeof(GLfloat));
}

void _math_matrix_free(SGLmatrix* mat)
{
  assert(mat->m != NULL);
  free(mat->m);
}

void _math_matrix_alloc_inv(SGLmatrix* mat)
{
  if (!mat->inv) {
    mat->inv = (GLfloat*) malloc(sizeof(GLfloat) * 16);
    memcpy(mat->inv, Identity, 16 * sizeof(GLfloat));
  }
}


#define A(row, col) a[(col<<2)+row]
#define B(row, col) b[(col<<2)+row]
#define P(row, col) product[(col<<2)+row]

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

static void matrix_multf(SGLmatrix* mat, const GLfloat* m)
{
  matmul4(mat->m, mat->m, m);
}

#define MAT(m,r,c) (m)[(c<<2)+(r)]
#define SWAP_ROWS(a, b) { GLfloat *_tmp = a; (a)=(b); (b)=_tmp; }

static GLboolean _math_matrix_invert(SGLmatrix* mat)
{
  _math_matrix_alloc_inv(mat);
   const GLfloat *m = mat->m;
   GLfloat *out = mat->inv;
   GLfloat wtmp[4][8];
   GLfloat m0, m1, m2, m3, s;
   GLfloat *r0, *r1, *r2, *r3;

   r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

   r0[0] = MAT(m,0,0), r0[1] = MAT(m,0,1),
   r0[2] = MAT(m,0,2), r0[3] = MAT(m,0,3),
   r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,

   r1[0] = MAT(m,1,0), r1[1] = MAT(m,1,1),
   r1[2] = MAT(m,1,2), r1[3] = MAT(m,1,3),
   r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,

   r2[0] = MAT(m,2,0), r2[1] = MAT(m,2,1),
   r2[2] = MAT(m,2,2), r2[3] = MAT(m,2,3),
   r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,

   r3[0] = MAT(m,3,0), r3[1] = MAT(m,3,1),
   r3[2] = MAT(m,3,2), r3[3] = MAT(m,3,3),
   r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

   /* choose pivot - or die */
   if (fabsf(r3[0])>fabsf(r2[0])) SWAP_ROWS(r3, r2);
   if (fabsf(r2[0])>fabsf(r1[0])) SWAP_ROWS(r2, r1);
   if (fabsf(r1[0])>fabsf(r0[0])) SWAP_ROWS(r1, r0);
   if (0.0 == r0[0])  return GL_FALSE;

   /* eliminate first variable     */
   m1 = r1[0]/r0[0]; m2 = r2[0]/r0[0]; m3 = r3[0]/r0[0];
   s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
   s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
   s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
   s = r0[4];
   if (s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
   s = r0[5];
   if (s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
   s = r0[6];
   if (s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
   s = r0[7];
   if (s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

   /* choose pivot - or die */
   if (fabsf(r3[1])>fabsf(r2[1])) SWAP_ROWS(r3, r2);
   if (fabsf(r2[1])>fabsf(r1[1])) SWAP_ROWS(r2, r1);
   if (0.0 == r1[1])  return GL_FALSE;

   /* eliminate second variable */
   m2 = r2[1]/r1[1]; m3 = r3[1]/r1[1];
   r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
   r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
   s = r1[4]; if (0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
   s = r1[5]; if (0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
   s = r1[6]; if (0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
   s = r1[7]; if (0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }

   /* choose pivot - or die */
   if (fabsf(r3[2])>fabsf(r2[2])) SWAP_ROWS(r3, r2);
   if (0.0 == r2[2])  return GL_FALSE;

   /* eliminate third variable */
   m3 = r3[2]/r2[2];
   r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
   r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6],
   r3[7] -= m3 * r2[7];

   /* last check */
   if (0.0 == r3[3]) return GL_FALSE;

   s = 1.0F/r3[3];             /* now back substitute row 3 */
   r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

   m2 = r2[3];                 /* now back substitute row 2 */
   s  = 1.0F/r2[2];
   r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
   r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
   m1 = r1[3];
   r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
   r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
   m0 = r0[3];
   r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
   r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

   m1 = r1[2];                 /* now back substitute row 1 */
   s  = 1.0F/r1[1];
   r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
   r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
   m0 = r0[2];
   r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
   r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

   m0 = r0[1];                 /* now back substitute row 0 */
   s  = 1.0F/r0[0];
   r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
   r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

   MAT(out,0,0) = r0[4]; MAT(out,0,1) = r0[5],
   MAT(out,0,2) = r0[6]; MAT(out,0,3) = r0[7],
   MAT(out,1,0) = r1[4]; MAT(out,1,1) = r1[5],
   MAT(out,1,2) = r1[6]; MAT(out,1,3) = r1[7],
   MAT(out,2,0) = r2[4]; MAT(out,2,1) = r2[5],
   MAT(out,2,2) = r2[6]; MAT(out,2,3) = r2[7],
   MAT(out,3,0) = r3[4]; MAT(out,3,1) = r3[5],
   MAT(out,3,2) = r3[6]; MAT(out,3,3) = r3[7];

   return GL_TRUE;
}

void _math_matrix_mul_matrix(SGLmatrix* product, const SGLmatrix* a,
                            const SGLmatrix* b)
{
  matmul4(product->m, a->m, b->m);
}

void _math_matrix_mul_floats(SGLmatrix* product, const GLfloat* b)
{
  matmul4(product->m, product->m, b);
}

void _math_matrix_loadf(SGLmatrix* mat, const GLfloat* m)
{
  memcpy(mat->m, m, 16 * sizeof(GLfloat));
}

void _math_matrix_translate(SGLmatrix* mat, GLfloat x , GLfloat y ,GLfloat z)
{
  GLfloat *m = mat->m;
  m[12] = x * m[0] + y * m[4] + z * m[8] + m[12]; 
  m[13] = x * m[1] + y * m[5] + z * m[9] + m[13];
  m[14] = x * m[2] + y * m[6] + z * m[10] + m[14];
  m[15] = x * m[3] + y * m[7] + z * m[11] + m[15];
}

void _math_matrix_rotate(SGLmatrix* mat, GLfloat angle, GLfloat x,
                         GLfloat y, GLfloat z)
{
  GLfloat xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c, s, c;
  GLfloat m[16];
  GLboolean optimized;

  s = (GLfloat) sin(D2R(angle));
  c = (GLfloat) cos(D2R(angle));

  memcpy(m, Identity, sizeof(GLfloat) * 16);
  optimized = GL_FALSE;

#define M(row,col)  m[col*4+row]

  if (x == 0.0F) {
    if (y == 0.0F) {
      if (z != 0.0F) {
        optimized = GL_TRUE;
        /* rotate only around z-axis */
        M(0,0) = c;
        M(1,1) = c;
        if (z < 0.0F) {
          M(0,1) = s;
          M(1,0) = -s;
        }
        else {
          M(0,1) = -s;
          M(1,0) = s;
        }
      }
    }
    else if (z == 0.0F) {
      optimized = GL_TRUE;
      /* rotate only around y-axis */
      M(0,0) = c;
      M(2,2) = c;
      if (y < 0.0F) {
        M(0,2) = -s;
        M(2,0) = s;
      }
      else {
        M(0,2) = s;
        M(2,0) = -s;
      }
    }
  }
  else if (y == 0.0F) {
    if (z == 0.0F) {
      optimized = GL_TRUE;
      /* rotate only around x-axis */
      M(1,1) = c;
      M(2,2) = c;
      if (x < 0.0F) {
        M(1,2) = s;
        M(2,1) = -s;
      }
      else {
        M(1,2) = -s;
        M(2,1) = s;
      }
    }
  }

  if (!optimized) {
    const GLfloat mag = sqrtf(x * x + y * y + z * z);

    if (mag <= 1.0e-4) {
      /* no rotation, leave mat as-is */
      return;
    }

    x /= mag;
    y /= mag;
    z /= mag;


    /*
     *     Arbitrary axis rotation matrix.
     *
     *  This is composed of 5 matrices, Rz, Ry, T, Ry', Rz', multiplied
     *  like so:  Rz * Ry * T * Ry' * Rz'.  T is the final rotation
     *  (which is about the X-axis), and the two composite transforms
     *  Ry' * Rz' and Rz * Ry are (respectively) the rotations necessary
     *  from the arbitrary axis to the X-axis then back.  They are
     *  all elementary rotations.
     *
     *  Rz' is a rotation about the Z-axis, to bring the axis vector
     *  into the x-z plane.  Then Ry' is applied, rotating about the
     *  Y-axis to bring the axis vector parallel with the X-axis.  The
     *  rotation about the X-axis is then performed.  Ry and Rz are
     *  simply the respective inverse transforms to bring the arbitrary
     *  axis back to its original orientation.  The first transforms
     *  Rz' and Ry' are considered inverses, since the data from the
     *  arbitrary axis gives you info on how to get to it, not how
     *  to get away from it, and an inverse must be applied.
     *
     *  The basic calculation used is to recognize that the arbitrary
     *  axis vector (x, y, z), since it is of unit length, actually
     *  represents the sines and cosines of the angles to rotate the
     *  X-axis to the same orientation, with theta being the angle about
     *  Z and phi the angle about Y (in the order described above)
     *  as follows:
     *
     *  cos ( theta ) = x / sqrt ( 1 - z^2 )
     *  sin ( theta ) = y / sqrt ( 1 - z^2 )
     *
     *  cos ( phi ) = sqrt ( 1 - z^2 )
     *  sin ( phi ) = z
     *
     *  Note that cos ( phi ) can further be inserted to the above
     *  formulas:
     *
     *  cos ( theta ) = x / cos ( phi )
     *  sin ( theta ) = y / sin ( phi )
     *
     *  ...etc.  Because of those relations and the standard trigonometric
     *  relations, it is pssible to reduce the transforms down to what
     *  is used below.  It may be that any primary axis chosen will give the
     *  same results (modulo a sign convention) using thie method.
     *
     *  Particularly nice is to notice that all divisions that might
     *  have caused trouble when parallel to certain planes or
     *  axis go away with care paid to reducing the expressions.
     *  After checking, it does perform correctly under all cases, since
     *  in all the cases of division where the denominator would have
     *  been zero, the numerator would have been zero as well, giving
     *  the expected result.
     */

    xx = x * x;
    yy = y * y;
    zz = z * z;
    xy = x * y;
    yz = y * z;
    zx = z * x;
    xs = x * s;
    ys = y * s;
    zs = z * s;
    one_c = 1.0F - c;

    /* We already hold the identity-matrix so we can skip some statements */
    M(0,0) = (one_c * xx) + c;
    M(0,1) = (one_c * xy) - zs;
    M(0,2) = (one_c * zx) + ys;
    /*    M(0,3) = 0.0F; */

    M(1,0) = (one_c * xy) + zs;
    M(1,1) = (one_c * yy) + c;
    M(1,2) = (one_c * yz) - xs;
    /*    M(1,3) = 0.0F; */

    M(2,0) = (one_c * zx) - ys;
    M(2,1) = (one_c * yz) + xs;
    M(2,2) = (one_c * zz) + c;
    /*    M(2,3) = 0.0F; */

    /*
       M(3,0) = 0.0F;
       M(3,1) = 0.0F;
       M(3,2) = 0.0F;
       M(3,3) = 1.0F;
       */
  }
#undef M

  matrix_multf(mat, m);
}

void _math_matrix_scale(SGLmatrix* mat, GLfloat x, GLfloat y, GLfloat z)
{
  GLfloat *m = mat->m;
   m[0] *= x;   m[4] *= y;   m[8]  *= z;
   m[1] *= x;   m[5] *= y;   m[9]  *= z;
   m[2] *= x;   m[6] *= y;   m[10] *= z;
   m[3] *= x;   m[7] *= y;   m[11] *= z;
}

void _math_matrix_ortho(SGLmatrix* mat,
                        GLfloat left, GLfloat right,
                        GLfloat bottom, GLfloat top,
                        GLfloat nearval, GLfloat farval)
{
   GLfloat m[16];

#define M(row,col)  m[(col<<2)+row]
   M(0,0) = 2.0F / (right-left);
   M(0,1) = 0.0F;
   M(0,2) = 0.0F;
   M(0,3) = -(right+left) / (right-left);

   M(1,0) = 0.0F;
   M(1,1) = 2.0F / (top-bottom);
   M(1,2) = 0.0F;
   M(1,3) = -(top+bottom) / (top-bottom);

   M(2,0) = 0.0F;
   M(2,1) = 0.0F;
   M(2,2) = -2.0F / (farval-nearval);
   M(2,3) = -(farval+nearval) / (farval-nearval);

   M(3,0) = 0.0F;
   M(3,1) = 0.0F;
   M(3,2) = 0.0F;
   M(3,3) = 1.0F;
#undef M

   matrix_multf(mat, m);
}

void _math_matrix_frustum(SGLmatrix* mat,
                          GLfloat left, GLfloat right,
                          GLfloat bottom, GLfloat top,
                          GLfloat nearval, GLfloat farval)
{
   GLfloat x, y, a, b, c, d;
   GLfloat m[16];

   x = (2.0F*nearval) / (right-left);
   y = (2.0F*nearval) / (top-bottom);
   a = (right+left) / (right-left);
   b = (top+bottom) / (top-bottom);
   c = -(farval+nearval) / (farval-nearval);
   d = -(2.0F*farval*nearval) / (farval-nearval);  /* error? */

#define M(row,col)  m[(col<<2)+row]
   M(0,0) = x;     M(0,1) = 0.0F;  M(0,2) = a;      M(0,3) = 0.0F;
   M(1,0) = 0.0F;  M(1,1) = y;     M(1,2) = b;      M(1,3) = 0.0F;
   M(2,0) = 0.0F;  M(2,1) = 0.0F;  M(2,2) = c;      M(2,3) = d;
   M(3,0) = 0.0F;  M(3,1) = 0.0F;  M(3,2) = -1.0F;  M(3,3) = 0.0F;
#undef M

   matrix_multf(mat, m);
}

void _math_matrix_viewport(SGLmatrix *m, GLint x, GLint y,
                           GLint width, GLint height,
                           GLfloat zNear, GLfloat zFar, GLfloat depthMax)
{
  _math_matrix_set_identity(m);
  m->m[MAT_SX] = (GLfloat) width / 2.0F;
  m->m[MAT_TX] = m->m[MAT_SX] + x;
  m->m[MAT_SY] = (GLfloat) height / 2.0F;
  m->m[MAT_TY] = m->m[MAT_SY] + y;
  m->m[MAT_SZ] = depthMax * ((zFar - zNear) / 2.0F);
  m->m[MAT_TZ] = depthMax * ((zFar - zNear) / 2.0F + zNear);
}

void _math_matrix_set_identity(SGLmatrix* mat)
{
  memcpy(mat->m, Identity, 16 * sizeof(GLfloat));
}

void _math_matrix_copy(SGLmatrix* to, const SGLmatrix* from)
{
  memcpy(to->m, from->m, 16 * sizeof(GLfloat));
  to->flags = from->flags;

  if (to->inv != 0) {
    if (from->inv == 0)
      _math_matrix_invert(to);
    else
      memcpy(to->inv, from->inv, 16 * sizeof(GLfloat));
  }
}

void _sgl_affine_transform(SGLmatrix* mat, GLfloat* vec)
{
  GLfloat v0, v1, v2, v3;
#define M(row, col) mat->m[(col<<2)+row]
  v0 = vec[0]; v1 = vec[1]; v2 = vec[2]; v3 = vec[3];
  vec[0] = M(0, 0) * v0 + M(0, 1) * v1 + M(0, 2) * v2 + M(0, 3) * v3;
  vec[1] = M(1, 0) * v0 + M(1, 1) * v1 + M(1, 2) * v2 + M(1, 3) * v3;
  vec[2] = M(2, 0) * v0 + M(2, 1) * v1 + M(2, 2) * v2 + M(2, 3) * v3;
  vec[3] = M(3, 0) * v0 + M(3, 1) * v1 + M(3, 2) * v2 + M(3, 3) * v3;
  vec[0] /= vec[3];
  vec[1] /= vec[3];
  vec[2] /= vec[3];
#undef M
}

void _math_matrix_print(SGLmatrix* mat)
{
  int i, j;
#define M(row, col) mat->m[(col<<2)+row]
  printf("%p: begin matrix\n", mat);
  for (i = 0; i < 4; ++i) {
    for (j = 0; j < 4; ++j)
      printf("%6.2f ", M(i, j));
    printf("\n");
  }
  printf("end matrix\n");
#undef M
}
