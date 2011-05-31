#ifndef __SGL_SGLMATRIX_H__
#define __SGL_SGLMATRIX_H__

#include "sglHeaders.h"

extern void glMatrixMode(GLenum mode);

extern void glPushMatrix(void);

extern void glPopMatrix(void);

extern void glLoadIdentity(void);

extern void glLoadMatrixd(const GLdouble* m);

extern void glLoadMatrixf(const GLfloat* m);

extern void glTranslatef(GLfloat x, GLfloat y, GLfloat z);

extern void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

extern void glScalef(GLfloat x, GLfloat y, GLfloat z);

#endif /* __SGL_SGLMATRIX_H__ */
