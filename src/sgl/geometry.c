#include "geometry.h"

#include "context.h"
#include "logging.h"
#include "macros.h"
#include "pipeline.h"
#include "math/m_vector.h"
#include "math/m_matrix.h"

void glBegin(GLenum mode)
{
  GET_CURRENT_CONTEXT(ctx);

  switch (mode) {
  case GL_POINTS:
  case GL_LINES:
  case GL_TRIANGLES:
  case GL_QUADS:
    break;
  default:
    _sgl_error(ctx, GL_INVALID_ENUM, "glBegin(): Invalid mode\n");
    return;
  }

  ctx->render_state.type = 0;
  ctx->render_state.current_exec_primitive = mode;
  ctx->render_state.transform_dirty = GL_FALSE;
  _math_matrix_mul_matrix(&ctx->model_projection_matrix,
                          &ctx->viewport.window_map,
                          ctx->projection_matrix_stack.top);
  _math_matrix_mul_matrix(&ctx->model_projection_matrix,
                          &ctx->model_projection_matrix,
                          ctx->modelview_matrix_stack.top);
}

void glEnd(void)
{
  GET_CURRENT_CONTEXT(ctx);
  _sgl_pipeline_iteration();
  ctx->render_state.current_exec_primitive = PRIM_OUTSIDE_BEGIN_END;
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
  data[3] = 1;
  _sgl_affine_transform(&ctx->model_projection_matrix, data);
  _math_vector4f_push_back(&ctx->vector_point, data, 4);
  _math_vector4f_push_back(&ctx->vector_color, ctx->primitive.color, 4);
  _math_vector4f_push_back(&ctx->vector_normal, ctx->primitive.normal, 4);
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
  data[3] = 1;
  _sgl_affine_transform(&ctx->model_projection_matrix, data);
  _math_vector4f_push_back(&ctx->vector_point, data, 4);
  _math_vector4f_push_back(&ctx->vector_color, ctx->primitive.color, 4);
  _math_vector4f_push_back(&ctx->vector_normal, ctx->primitive.normal, 4);
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
  ctx->primitive.color[0] = CLAMP(red, 0, 1.0);
  ctx->primitive.color[1] = CLAMP(green, 0, 1.0);
  ctx->primitive.color[2] = CLAMP(blue, 0, 1.0);
  ctx->primitive.color[3] = 1;
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
  ctx->primitive.color[0] = CLAMP(red, 0, 1.0);
  ctx->primitive.color[1] = CLAMP(green, 0, 1.0);
  ctx->primitive.color[2] = CLAMP(blue, 0, 1.0);
  ctx->primitive.color[3] = CLAMP(alpha, 0, 1.0);
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
  _sgl_affine_transform(&ctx->model_projection_matrix, data);
  ctx->primitive.normal[0] = data[0];
  ctx->primitive.normal[1] = data[1];
  ctx->primitive.normal[2] = data[2];
  ctx->primitive.normal[3] = data[3];
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

void glDrawElements(GLenum mode, GLsizei count,
                    GLenum type, const GLvoid *indices)
{
  GET_CURRENT_CONTEXT(ctx);
  ASSERT_OUTSIDE_BEGIN_END_AND_FLUSH(ctx);

  if (!(ctx->clientstate_flags & GL_VERTEX_ARRAY))
    return;

  _math_matrix_mul_matrix(&ctx->model_projection_matrix,
                          &ctx->viewport.window_map,
                          ctx->projection_matrix_stack.top);
  _math_matrix_mul_matrix(&ctx->model_projection_matrix,
                          &ctx->model_projection_matrix,
                          ctx->modelview_matrix_stack.top);
  switch (mode) {
  case GL_POINTS:
  case GL_LINES:
  case GL_TRIANGLES:
  case GL_QUADS:
    break;
  default:
    _sgl_error(ctx, GL_INVALID_ENUM, "glDrawElements(): Invalid mode\n");
    return;
  }

  ctx->vertex_pointer.count = count;
  ctx->render_state.current_exec_primitive = mode;
  ctx->render_state.type = GL_VERTEX_ARRAY;
  ctx->render_state.transform_dirty = GL_TRUE;
  _sgl_pipeline_iteration();
  ctx->render_state.current_exec_primitive = PRIM_OUTSIDE_BEGIN_END;
}
