/**
 * @file m_vector.h
 *
 * Vector Math Library
 *
 * Trimmed and partially modified from the Mesa 3D project
 * Copyright (C) 1999-2005  Brian Paul   All Rights Reserved.
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

#ifndef __SGL_MATH_VECTOR_H__
#define __SGL_MATH_VECTOR_H__

#include "sglheader.h"

#define VEC_MALLOC 0x1

typedef struct _SGLvector4f SGLvector4f;
struct _SGLvector4f
{
  GLfloat (*data)[4];     /* point to malloced data or client data */
  GLfloat* start;         /* points somewhere inside of <data> */
  GLuint count;           /* size of the vector (in elements) */
  GLuint stride;          /* stride from one element to the next (in bytes) */
  GLuint size;            /* element size 2-4 */
  void* storage;          /* self-allocated storage */
  GLuint storage_count;   /* storage size in elements */
  GLbitfield flags;       /* VEC_X flags */
};

/**
 * @brief Initialize vector with storage
 * @param v points to a SGLvector4f
 * @param flags flags to set for SGLvector4f
 * @param storage point to the vector storage
 */
extern void _math_vector4f_init(SGLvector4f *v, GLbitfield flags,
                                GLfloat (*storage)[4]);

/**
 * @brief Initialize vector and allocate <count> storage of elemnts
 * @param v points to a SGLvector4f
 * @param flags flags to set for SGLvector4f
 * @param count number of element to be allocated
 */
extern void _math_vector4f_alloc(SGLvector4f *v, GLbitfield flags,
                                 GLuint count);

/**
 * @brief Double the storage of an existing self-allocated vector
 * @param v points to a vector
 */
extern void _math_vector4f_alloc_double(SGLvector4f *v);

/**
 * @brief free storage for a SGLvector4f
 * @param v points to a SGLvector4f
 */
extern void _math_vector4f_free(SGLvector4f* v);

/**
 * @brief lazy free
 * @param v points to a SGLvector4f
 */
extern void _math_vector4f_lazy_free(SGLvector4f* v);

/**
 * @brief push a vector into a SGLvector4f
 * @param v points to a SGLvector4f
 * @param data point to a vector
 * @param size the size of <data>
 * @return return GL_TRUE on success
 */
extern GLboolean _math_vector4f_push_back(SGLvector4f* v, GLfloat* data,
                                          GLuint size);

/**
 * @brief pop last vector from a SGLvector4f
 * @param v points to a SGLvector4f
 * @param data place to store the popped data, allow NULL
 * @param size the size of <data>
 * @return return GL_TRUE on success
 */
extern GLboolean _math_vector4f_pop_back(SGLvector4f* v, GLfloat* data,
                                         GLuint size);

extern GLboolean _math_vector4f_empty(SGLvector4f* v);

extern void _math_vector4f_print(SGLvector4f* v);

/**
 * Given vector <v>, return <type> pointer of the <i>-th element.
 */
#define VEC_ELT(v, type, i) \
        ((type*) ((GLbyte*) ((v)->data) + (i) * (v)->stride))

#endif /* __SGL_MATH_VECTOR_H__ */
