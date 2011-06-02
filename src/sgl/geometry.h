#ifndef __SGL_GEOMETRY_H__
#define __SGL_GEOMETRY_H__

#include "sglheader.h"

/* glVertex */
extern void glVertex3s(GLshort x, GLshort y, GLshort z);
extern void glVertex3i(GLint x, GLint y, GLint z);
extern void glVertex3f(GLfloat x, GLfloat y, GLfloat z);
extern void glVertex3d(GLdouble x, GLdouble y, GLdouble z);

extern void glVertex4s(GLshort x, GLshort y, GLshort z, GLshort w);
extern void glVertex4i(GLint x, GLint y, GLint z, GLint w);
extern void glVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void glVertex4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w);

/* glColor */
extern void glColor3d(GLdouble red, GLdouble green, GLdouble blue);
extern void glColor3f(GLfloat red, GLfloat green, GLfloat blue);
extern void glColor3i(GLint red, GLint green, GLint blue);
extern void glColor3s(GLshort red, GLshort green, GLshort blue);

extern void glColor4d(GLdouble red, GLdouble green, GLdouble blue,
                      GLdouble alpha);
extern void glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern void glColor4i(GLint red, GLint green, GLint blue, GLint alpha);
extern void glColor4s(GLshort red, GLshort green, GLshort blue, GLshort alpha);

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
