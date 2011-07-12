/**
 * @file   sgl/geometry.h
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
 */

#ifndef __SGL_GEOMETRY_H__
#define __SGL_GEOMETRY_H__

#include "sglheader.h"

/* glVertex */
extern void glVertex3s(GLshort x, GLshort y, GLshort z);
extern void glVertex3i(GLint x, GLint y, GLint z);
extern void glVertex3f(GLfloat x, GLfloat y, GLfloat z);
extern void glVertex3d(GLdouble x, GLdouble y, GLdouble z);

extern void glVertex3dv(const GLdouble *v);
extern void glVertex3fv(const GLfloat *v);
extern void glVertex3iv(const GLint *v);
extern void glVertex3sv(const GLshort *v);

extern void glVertex4s(GLshort x, GLshort y, GLshort z, GLshort w);
extern void glVertex4i(GLint x, GLint y, GLint z, GLint w);
extern void glVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void glVertex4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w);

extern void glVertex4dv(const GLdouble *v);
extern void glVertex4fv(const GLfloat *v);
extern void glVertex4iv(const GLint *v);
extern void glVertex4sv(const GLshort *v);

/* glColor */
extern void glColor3d(GLdouble red, GLdouble green, GLdouble blue);
extern void glColor3f(GLfloat red, GLfloat green, GLfloat blue);
extern void glColor3i(GLint red, GLint green, GLint blue);
extern void glColor3s(GLshort red, GLshort green, GLshort blue);

extern void glColor3dv(const GLdouble *v);
extern void glColor3fv(const GLfloat *v);
extern void glColor3iv(const GLint *v);
extern void glColor3sv(const GLshort *v);

extern void glColor4d(GLdouble red, GLdouble green, GLdouble blue,
                      GLdouble alpha);
extern void glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern void glColor4i(GLint red, GLint green, GLint blue, GLint alpha);
extern void glColor4s(GLshort red, GLshort green, GLshort blue, GLshort alpha);

extern void glColor4dv(const GLdouble *v);
extern void glColor4fv(const GLfloat *v);
extern void glColor4iv(const GLint *v);
extern void glColor4sv(const GLshort *v);

/* glNormal */
extern void glNormal3d(GLdouble nx, GLdouble ny, GLdouble nz);
extern void glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz);
extern void glNormal3i(GLint nx, GLint ny, GLint nz);
extern void glNormal3s(GLshort nx, GLshort ny, GLshort nz);

extern void glNormal3dv(const GLdouble *v);
extern void glNormal3fv(const GLfloat *v);
extern void glNormal3iv(const GLint *v);
extern void glNormal3sv(const GLshort *v);

/* Pointer */
extern void glVertexPointer(GLint size, GLenum type, GLsizei stride,
                            const GLvoid *ptr);
extern void glNormalPointer(GLenum type, GLsizei stride, const GLvoid *ptr);
extern void glColorPointer(GLint size, GLenum type, GLsizei stride,
                           const GLvoid *ptr);
extern void glDrawElements(GLenum mode, GLsizei count,
                           GLenum type, const GLvoid *indices);

#endif /* __SGL_GEOMETRY_H__ */
