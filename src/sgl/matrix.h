/**
 * @file   matrix.h
 *
 * Copyright (C) 2011 - Wei-Ning Huang <aitjcize@gmail.com>
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
 */

#ifndef __SGL_MATRIX_H__
#define __SGL_MATRIX_H__

#include "sglheader.h"

extern void _sgl_init_matrix(struct sgl_context* ctx);

extern void _sgl_free_matrix_data(struct sgl_context* ctx);

extern void glMatrixMode(GLenum mode);

extern void glPushMatrix(void);
extern void glPopMatrix(void);

extern void glLoadIdentity(void);

extern void glFrustum(GLdouble left, GLdouble right,
                      GLdouble bottom, GLdouble top,
                      GLdouble nearval, GLdouble farval);

extern void glOrtho(GLdouble left, GLdouble right,
                    GLdouble bottom, GLdouble top,
                    GLdouble nearval, GLdouble farval);

extern void glMultMatrixd(const GLdouble* m);
extern void glMultMatrixf(const GLfloat* m);

extern void glLoadMatrixd(const GLdouble* m);
extern void glLoadMatrixf(const GLfloat* m);

extern void glTranslated(GLdouble x, GLdouble y, GLdouble z);
extern void glTranslatef(GLfloat x, GLfloat y, GLfloat z);

extern void glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
extern void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

extern void glScaled(GLdouble x, GLdouble y, GLdouble z);
extern void glScalef(GLfloat x, GLfloat y, GLfloat z);

#endif /* __SGL_MATRIX_H__ */
