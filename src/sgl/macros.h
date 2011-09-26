/**
 * @file   macros.h
 *
 * Copyright (C) 2011 - SGL Authors <aitjcize@gmail.com>
 * All Rights reserved.
 *
 * Some macros are taken from the Mesa 3D project
 * Copyright (C) 1999-2006  Brian Paul   All Rights Reserved.
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
 */

#ifndef __SGL_MACROS_H__
#define __SGL_MACROS_H__

/* Clamp X to [MIN,MAX] */
#define CLAMP(X, MIN, MAX)  ((X)<(MIN) ? (MIN) : ((X)>(MAX) ? (MAX) : (X)))

/* Clamp X to [0,255] */
#define CCLAMP(X)  ((GLuint)((X) * 255) & 0xff)

/* Minimum of two values: */
#define MIN2( A, B )   ( (A)<(B) ? (A) : (B) )

/* Maximum of two values: */
#define MAX2( A, B )   ( (A)>(B) ? (A) : (B) )

/* Dot product of two 2-element vectors */
#define DOT2( a, b )  ( (a)[0]*(b)[0] + (a)[1]*(b)[1] )

/* Dot product of two 3-element vectors */
#define DOT3( a, b )  ( (a)[0]*(b)[0] + (a)[1]*(b)[1] + (a)[2]*(b)[2] )

/* Dot product of two 4-element vectors */
#define DOT4( a, b )  ( (a)[0]*(b)[0] + (a)[1]*(b)[1] + \
            (a)[2]*(b)[2] + (a)[3]*(b)[3] )

/* Dot product of two 4-element vectors */
#define DOT4V(v,a,b,c,d) (v[0]*(a) + v[1]*(b) + v[2]*(c) + v[3]*(d))


/* Cross product of two 3-element vectors */
#define CROSS3(n, u, v)                     \
do {                                        \
   (n)[0] = (u)[1]*(v)[2] - (u)[2]*(v)[1];  \
   (n)[1] = (u)[2]*(v)[0] - (u)[0]*(v)[2];  \
   (n)[2] = (u)[0]*(v)[1] - (u)[1]*(v)[0];  \
} while (0)


/* Normalize a 3-element vector to unit length. */
#define NORMALIZE_3FV( V )                      \
do {                                            \
   GLfloat len = (GLfloat) LEN_SQUARED_3FV(V);  \
   if (len) {                                   \
      len = INV_SQRTF(len);                     \
      (V)[0] = (GLfloat) ((V)[0] * len);        \
      (V)[1] = (GLfloat) ((V)[1] * len);        \
      (V)[2] = (GLfloat) ((V)[2] * len);        \
   }                                            \
} while(0)

#define LEN_2(x, y) (sqrtf((x)*(x) + (y)*(y)))

#define LEN_3FV( V ) (sqrtf((V)[0]*(V)[0]+(V)[1]*(V)[1]+(V)[2]*(V)[2]))
#define LEN_2FV( V ) (sqrtf((V)[0]*(V)[0]+(V)[1]*(V)[1]))

#define LEN_SQUARED_3FV( V ) ((V)[0]*(V)[0]+(V)[1]*(V)[1]+(V)[2]*(V)[2])
#define LEN_SQUARED_2FV( V ) ((V)[0]*(V)[0]+(V)[1]*(V)[1])

#define COLOR_FF(c)             \
  ((CCLAMP((c)[3]) << 24) |     \
   (CCLAMP((c)[0]) << 16) |     \
   (CCLAMP((c)[1]) <<  8) |     \
   (CCLAMP((c)[2])))

#define COLOR_FF_CT(cc)                  \
 (__extension__({                        \
   color_t __rc;                         \
   __rc.c.a = (GLubyte)CCLAMP((cc)[2]);  \
   __rc.c.r = (GLubyte)CCLAMP((cc)[1]);  \
   __rc.c.g = (GLubyte)CCLAMP((cc)[0]);  \
   __rc.c.b = (GLubyte)CCLAMP((cc)[3]);  \
   __rc;                                 \
 }))

#define COLOR_FE(r, g, b, a) \
  ((CCLAMP(a) << 24) | (CCLAMP(r) << 16) | (CCLAMP(g) <<  8) | (CCLAMP(b)))

#define LINEAR_IP(val1, val2, f, a) \
  ((val1) + ((val2) - (val1)) * (GLfloat)f / a)

#define COLOR_IP(cc1, cc2, f, a)                  \
 (__extension__({                                 \
   color_t __rc;                                  \
   __rc.c.a = LINEAR_IP(cc1.c.a, cc2.c.a, f, a);  \
   __rc.c.r = LINEAR_IP(cc1.c.r, cc2.c.r, f, a);  \
   __rc.c.g = LINEAR_IP(cc1.c.g, cc2.c.g, f, a);  \
   __rc.c.b = LINEAR_IP(cc1.c.b, cc2.c.b, f, a);  \
   __rc.val;                                      \
 }))

#define COLOR_WSUM(a, cc1, b, cc2, c, cc3)                         \
  (__extension__({                                                 \
    color_t __rc;                                                  \
    __rc.c.a = (GLubyte)(a * cc1.c.a + b * cc2.c.a + c * cc3.c.a); \
    __rc.c.r = (GLubyte)(a * cc1.c.r + b * cc2.c.r + c * cc3.c.r); \
    __rc.c.g = (GLubyte)(a * cc1.c.g + b * cc2.c.g + c * cc3.c.g); \
    __rc.c.b = (GLubyte)(a * cc1.c.b + b * cc2.c.b + c * cc3.c.b); \
    __rc;                                                          \
  }))

#define DEPTH_WSUM(a, d1, b, d2, c, d3)                \
  (a * d1 + b * d2 + c * d3)

/* Framebuffer access related */
#define BUF_SET_C(buf, x, y, cc) \
    ((GLuint*)(buf)->data)[((buf)->height-y-1)*(buf)->width+x-1] = cc;

#define BUF_SET_D(buf, x, y, d) \
    ((GLfloat*)(buf)->data)[((buf)->height-y-1)*(buf)->width+x-1] = d;

#define BUF_GET_C(buf, x, y) \
  (((GLuint*)(buf)->data)[((buf)->height-(y)-1)*(buf)->width+(x)-1])

#define BUF_GET_D(buf, x, y) \
  (((GLfloat*)(buf)->data)[((buf)->height-(y)-1)*(buf)->width+(x)-1])

#define NORMALIZE_Z(ctx, z) \
  ((z - ctx->drawbuffer->depth_max * ctx->viewport.near) / \
   (ctx->drawbuffer->depth_max * (ctx->viewport.far - ctx->viewport.near)))

#define DNORMALIZE_Z(ctx, z) \
   (z * (ctx->drawbuffer->depth_max * (ctx->viewport.far-ctx->viewport.near)) \
     + ctx->drawbuffer->depth_max * ctx->viewport.near)

/* Edge Table related */
#define ET_GET(et, y, x) (et)[(y) * EDGE_TABLE_SIZE + (x)]

#define MOVE_FLOAT_4(dst, src) \
  do {                         \
    ((GLfloat*)(dst))[0] = ((GLfloat*)(src))[0];           \
    ((GLfloat*)(dst))[1] = ((GLfloat*)(src))[1];           \
    ((GLfloat*)(dst))[2] = ((GLfloat*)(src))[2];           \
    ((GLfloat*)(dst))[3] = ((GLfloat*)(src))[3];           \
  } while (0)

#endif /* __SGL_MACROS_H__ */
