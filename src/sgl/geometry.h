#ifndef __SGL_GEOMETRY_H__
#define __SGL_GEOMETRY_H__

#include "sgl_headers.h"

/* glVertex */
extern void glVertex3s(GLshort x, GLshort y, GLshort z);

extern void glVertex3i(GLint x, GLint y, GLint z);

extern void glVertex3f(GLfloat x, GLfloat y, GLfloat z);

extern void glVertex3d(GLdouble x, GLdouble y, GLdouble z);

/* glColor */
extern void glColor3d(GLdouble red, GLdouble green, GLdouble blue);

extern void glColor3f(GLfloat red, GLfloat green, GLfloat blue);

extern void glColor3i(GLint red, GLint green, GLint blue);

extern void glColor3s(GLshort red, GLshort green, GLshort blue);

/* glNormal */
extern void glNormal3d(GLdouble nx, GLdouble ny, GLdouble nz);

extern void glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz);

extern void glNormal3i(GLint nx, GLint ny, GLint nz);

extern void glNormal3s(GLshort nx, GLshort ny, GLshort nz);

/* Pointer */
extern void glVertexPointer(GLint size, GLenum type, GLsizei stride,
                            const GLvoid *ptr);

extern void glNormalPointer(GLenum type, GLsizei stride, const GLvoid *ptr);

extern void glColorPointer(GLint size, GLenum type, GLsizei stride,
                           const GLvoid *ptr);

#endif /* __SGL_GEOMETRY_H__ */
