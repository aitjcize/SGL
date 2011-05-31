#include "sglContext.h"
#include "sglGeometry.h"

extern SGLContext s_sgl;

void glVertex3s(GLshort x, GLshort y, GLshort z)
{
  glVertex3f(x, y, z);
}

void glVertex3i(GLint x, GLint y, GLint z)
{
  glVertex3f(x, y, z);
}

void glVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
  GLfloat data[3];
  data[0] = x;
  data[1] = y;
  data[2] = z;
  _sgl_vertex_push_back(s_sgl.vector_point, data, 3);
}

void glVertex3d(GLdouble x, GLdouble y, GLdouble z)
{
  glVertex3f(x, y, z);
}

void glColor3d(GLdouble red, GLdouble green, GLdouble blue)
{
  glColor3f(red, green, blue);
}

void glColor3f(GLfloat red, GLfloat green, GLfloat blue)
{
  GLfloat data[3];
  data[0] = red;
  data[1] = green;
  data[2] = blue;
  _sgl_vertex_push_back(s_sgl.vector_color, data, 3);
}

void glColor3i(GLint red, GLint green, GLint blue)
{
  glColor3f(red, green, blue);
}

void glColor3s(GLshort red, GLshort green, GLshort blue)
{
  glColor3f(red, green, blue);
}

void glNormal3d(GLdouble nx, GLdouble ny, GLdouble nz)
{
  glNormal3f(nx, ny, nz);
}

void glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz)
{
  GLfloat data[3];
  data[0] = nx;
  data[1] = ny;
  data[2] = nz;
  _sgl_vertex_push_back(s_sgl.vector_normal, data, 3);
}

void glNormal3i(GLint nx, GLint ny, GLint nz)
{
  glNormal3f(nx, ny, nz);
}

void glNormal3s(GLshort nx, GLshort ny, GLshort nz)
{
  glNormal3f(nx, ny, nz);
}

void glVertexPointer(GLint size, GLenum type, GLsizei stride,
                     const GLvoid *ptr)
{
  if (s_sgl.clientstate_flags & GL_VERTEX_ARRAY) {
    _sgl_vector4f_init(&s_sgl.vertex_pointer, 0, (GLvoid*)ptr);
    s_sgl.vertex_pointer.stride = stride;
  }
}

void glNormalPointer(GLenum type, GLsizei stride, const GLvoid *ptr)
{
  if (s_sgl.clientstate_flags & GL_NORMAL_ARRAY) {
    _sgl_vector4f_init(&s_sgl.normal_pointer, 0, (GLvoid*)ptr);
    s_sgl.vertex_pointer.stride = stride;
  }
}


void glColorPointer(GLint size, GLenum type, GLsizei stride,
                    const GLvoid *ptr)
{
  if (s_sgl.clientstate_flags & GL_COLOR_ARRAY) {
    _sgl_vector4f_init(&s_sgl.color_pointer, 0, (GLvoid*)ptr);
    s_sgl.vertex_pointer.stride = stride;
  }
}
