#ifndef __SGL_SGLMATRIX_OP_H__
#define __SGL_SGLMATRIX_OP_H__

#include "glHeader.h"

void glPushMatrix(void);

void glPopMatrix(void);

void glLoadIdentity(void);

void glLoadMatrixd(const GLdouble* m);

void glLoadMatrixf(const GLfloat* m);

void glTranslatef(GLfloat x, GLfloat y, GLfloat z);

void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

void glScalef(GLfloat x, GLfloat y, GLfloat z);

#endif /* __SGL_SGLMATRIX_OP_H__ */
