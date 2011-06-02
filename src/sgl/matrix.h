#ifndef __SGL_MATRIX_H__
#define __SGL_MATRIX_H__

#include "sgl_headers.h"

extern void _sgl_init_matrix(struct sgl_context* ctx);

extern void _sgl_free_matrix_data(struct sgl_context* ctx);

extern void glMatrixMode(GLenum mode);

extern void glPushMatrix(void);

extern void glPopMatrix(void);

extern void glLoadIdentity(void);

extern void glLoadMatrixd(const GLdouble* m);

extern void glLoadMatrixf(const GLfloat* m);

extern void glTranslatef(GLfloat x, GLfloat y, GLfloat z);

extern void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

extern void glScalef(GLfloat x, GLfloat y, GLfloat z);

#endif /* __SGL_MATRIX_H__ */
