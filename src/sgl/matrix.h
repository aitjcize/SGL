/**
 * @file   matrix.h
 *
 * Copyright (C) 2011 - SGL Authors <aitjcize@gmail.com>
 * All Rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
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
