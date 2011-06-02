#include "geometry.h"

#include "context.h"
#include "math/m_vector.h"

#define M(row, col) ((col<<2)+row)

void vector_transform(SGLmatrix* mat, GLfloat* vec) {
  GLfloat v0, v1, v2, v3;
  v0 = vec[0]; v1 = vec[1]; v2 = vec[2]; v3 = vec[3];
  vec[0] = M(0, 0) * v0 + M(0, 1) * v1 + M(0, 2) * v2 + M(0, 3) * v3;
  vec[1] = M(1, 0) * v0 + M(1, 1) * v1 + M(1, 2) * v2 + M(1, 3) * v3;
  vec[2] = M(2, 0) * v0 + M(2, 1) * v1 + M(2, 2) * v2 + M(2, 3) * v3;
  vec[3] = M(3, 0) * v0 + M(3, 1) * v1 + M(3, 2) * v2 + M(3, 3) * v3;
}

#undef M

void glBegin(GLenum mode)
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->flags |= mode;
}

void glEnd(void)
{
}

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
  GET_CURRENT_CONTEXT(ctx);
  GLfloat data[4];
  data[0] = x;
  data[1] = y;
  data[2] = z;
  data[3] = 0;
  vector_transform(ctx->modelview_matrix_stack.top, data);
  _math_vector4f_push_back(&ctx->vector_point, data, 4);
}

void glVertex3d(GLdouble x, GLdouble y, GLdouble z)
{
  glVertex3f(x, y, z);
}

void glVertex4s(GLshort x, GLshort y, GLshort z, GLshort w)
{
  glVertex4f(x, y, z, w);
}

void glVertex4i(GLint x, GLint y, GLint z, GLint w)
{
  glVertex4f(x, y, z, w);
}

void glVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
  GET_CURRENT_CONTEXT(ctx);
  GLfloat data[4];
  data[0] = x / w;
  data[1] = y / w;
  data[2] = z / w;
  data[3] = 0;
  vector_transform(ctx->modelview_matrix_stack.top, data);
  _math_vector4f_push_back(&ctx->vector_point, data, 4);
}

void glVertex4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
  glVertex4f(x, y, z, w);
}

void glColor3d(GLdouble red, GLdouble green, GLdouble blue)
{
  glColor3f(red, green, blue);
}

void glColor3f(GLfloat red, GLfloat green, GLfloat blue)
{
  GET_CURRENT_CONTEXT(ctx);
  GLfloat data[3];
  data[0] = red;
  data[1] = green;
  data[2] = blue;
  _math_vector4f_push_back(&ctx->vector_color, data, 3);
}

void glColor3i(GLint red, GLint green, GLint blue)
{
  glColor3f(red, green, blue);
}

void glColor3s(GLshort red, GLshort green, GLshort blue)
{
  glColor3f(red, green, blue);
}

void glColor4d(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha)
{
  glColor4f(red, green, blue, alpha);
}

void glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
  GET_CURRENT_CONTEXT(ctx);
  GLfloat data[4];
  data[0] = red;
  data[1] = green;
  data[2] = blue;
  data[3] = alpha;
  _math_vector4f_push_back(&ctx->vector_color, data, 4);
}

void glColor4i(GLint red, GLint green, GLint blue, GLint alpha)
{
  glColor4f(red, green, blue, alpha);
}

void glColor4s(GLshort red, GLshort green, GLshort blue, GLshort alpha)
{
  glColor4f(red, green, blue, alpha);
}

void glNormal3d(GLdouble nx, GLdouble ny, GLdouble nz)
{
  glNormal3f(nx, ny, nz);
}

void glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz)
{
  GET_CURRENT_CONTEXT(ctx);
  GLfloat data[4];
  data[0] = nx;
  data[1] = ny;
  data[2] = nz;
  data[3] = 1;
  vector_transform(ctx->modelview_matrix_stack.top, data);
  _math_vector4f_push_back(&ctx->vector_normal, data, 3);
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
  GET_CURRENT_CONTEXT(ctx);
  if (ctx->clientstate_flags & GL_VERTEX_ARRAY) {
    _math_vector4f_init(&ctx->vertex_pointer, 0, (GLvoid*)ptr);
    ctx->vertex_pointer.stride = stride;
  }
}

void glNormalPointer(GLenum type, GLsizei stride, const GLvoid *ptr)
{
  GET_CURRENT_CONTEXT(ctx);
  if (ctx->clientstate_flags & GL_NORMAL_ARRAY) {
    _math_vector4f_init(&ctx->normal_pointer, 0, (GLvoid*)ptr);
    ctx->vertex_pointer.stride = stride;
  }
}

void glColorPointer(GLint size, GLenum type, GLsizei stride,
                    const GLvoid *ptr)
{
  GET_CURRENT_CONTEXT(ctx);
  if (ctx->clientstate_flags & GL_COLOR_ARRAY) {
    _math_vector4f_init(&ctx->color_pointer, 0, (GLvoid*)ptr);
    ctx->vertex_pointer.stride = stride;
  }
}
