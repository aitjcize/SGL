#include "rasterize.h"

#include <stdio.h>
#include <string.h>

#include "logging.h"
#include "macros.h"
#include "types.h"
#include "math/m_matrix.h"
#include "math/m_vector.h"

#define BUF_DRAW(buf, x, y, c)                                                \
  do {                                                                        \
    if (x > 0 && y >= 0 && x <= buf->width && y < buf->height)                \
      ((GLuint*)buf->data)[(buf->height-y-1)*buf->width+x-1] = c;             \
  } while (0)

#define BUF_DRAW_F(buf, x, y, c) \
  do {                                                                        \
    if (x > 0 && y >= 0 && x <= buf->width && y < buf->height)                \
      ((GLuint*)buf->data)[(buf->height-y-1)*buf->width+x-1] = COLOR_FF(c);   \
  } while (0)

#define BUF_GET(buf, x, y) \
  (((GLuint*)buf->data)[(buf->height-(y)-1)*buf->width+(x)-1])

void _flood_fill(struct sgl_renderbuffer* buf, GLint sx, GLint sy,
                 GLfloat* color)
{
  GET_CURRENT_CONTEXT(ctx);
  if (sx < 0 || sy < 0 || sx > buf->width || sy > buf->height)
    return;

  GLuint data[3];
  GLuint cc = COLOR_FF(color);

  data[0] = sx; data[1] = sy; data[2] = cc;

  _math_vector4f_push_back(&ctx->flood_fill, (GLfloat*)data, 3);
  while (_math_vector4f_pop_back(&ctx->flood_fill, (GLfloat*)data, 3)) {
    sx = data[0]; sy = data[1]; cc = data[2];

    BUF_DRAW(buf, sx, sy, cc);
    data[2] = cc;
    if (sx + 1 <= buf->width && BUF_GET(buf, sx + 1, sy) != cc) {
      data[0] = sx + 1; data[1] = sy;
      _math_vector4f_push_back(&ctx->flood_fill, (GLfloat*)data, 3);
    }

    if (sx - 1 > 0 && BUF_GET(buf, sx - 1, sy) != cc) {
      data[0] = sx - 1; data[1] = sy;
      _math_vector4f_push_back(&ctx->flood_fill, (GLfloat*)data, 3);
    }

    if (sy + 1 < buf->height && BUF_GET(buf, sx, sy + 1) != cc) {
      data[0] = sx; data[1] = sy + 1;
      _math_vector4f_push_back(&ctx->flood_fill, (GLfloat*)data, 3);
    }

    if (sy - 1 >= 0 && BUF_GET(buf, sx, sy - 1) != cc) {
      data[0] = sx; data[1] = sy - 1;
      _math_vector4f_push_back(&ctx->flood_fill, (GLfloat*)data, 3);
    }
  }
  _math_vector4f_lazy_free(&ctx->flood_fill);
}

void _sgl_draw_point(struct sgl_renderbuffer* buf, GLfloat* point,
                     GLfloat* color)
{
  BUF_DRAW_F(buf, (GLint)point[0], (GLint)point[1], color);
}

void _sgl_draw_line(struct sgl_renderbuffer* buf, GLfloat* point,
                    GLfloat* color)
{
  GLint x1 = point[0], y1 = point[1],
        x2 = point[4], y2 = point[5];

  /* Bresenham's Algorithm */
  int dx, dy, i, err;
  int incx, incy, inc1, inc2;
  int x = x1; 
  int y = y1;
  int cc = COLOR_FF(color);

  dx = x2 - x1;
  dy = y2 - y1;

  if(dx < 0) dx = -dx;
  if(dy < 0) dy = -dy;

  if(x2 < x1)
    incx = -1;
  else
    incx = 1;

  if(y2 < y1)
    incy = -1;
  else
    incy = 1;

  /* Draw m <= 1 Line */
  if(dx > dy)
  {
    BUF_DRAW(buf, x, y, cc);
    err = (2 * dy) - dx;
    inc1 = 2 * (dy - dx);
    inc2 = 2 * dy;

    for(i = 0 ; i < dx ; ++i)
    {
      if(err >= 0)
      {
        y += incy;
        err += inc1;
      }
      else
        err += inc2;

      x += incx;
      BUF_DRAW(buf, x, y, cc);
    }
  }
  else
  {
    BUF_DRAW(buf, x, y, cc);
    err = (2 * dx) - dy;
    inc1 = 2 * (dx - dy);
    inc2 = 2 * dx;

    for(i = 0 ; i < dy ; ++i)
    {
      if(err >= 0)
      {
        x += incx;
        err += inc1;
      }
      else
        err += inc2;

      y += incy;
      BUF_DRAW(buf, x, y, cc);
    }
  }
}

void _sgl_draw_triangle(struct sgl_renderbuffer* buf, GLfloat* point,
                        GLfloat* color)
{
  GET_CURRENT_CONTEXT(ctx);
  GLfloat wrap_point[8];
  memcpy(wrap_point, point + 8, sizeof(GLfloat) * 4);
  memcpy(wrap_point + 4, point, sizeof(GLfloat) * 4);

  _sgl_draw_line(buf, point, color);
  _sgl_draw_line(buf, point + 4, color);
  _sgl_draw_line(buf, wrap_point, color);

  if (ctx->polygon.front == GL_FILL) {
    GLint sx = 0, sy = 0;
    sx = (point[0] + point[4] + point[8]) / 3;
    sy = (point[1] + point[5] + point[9]) / 3;
    _flood_fill(buf, sx, sy, color);
  }
}

void _sgl_draw_quads(struct sgl_renderbuffer* buf, GLfloat* point,
                     GLfloat* color)
{
  GET_CURRENT_CONTEXT(ctx);
  GLfloat wrap_point[8];
  memcpy(wrap_point, point + 12, sizeof(GLfloat) * 4);
  memcpy(wrap_point + 4, point, sizeof(GLfloat) * 4);

  _sgl_draw_line(buf, point, color);
  _sgl_draw_line(buf, point + 4, color);
  _sgl_draw_line(buf, point + 8, color);
  _sgl_draw_line(buf, wrap_point, color);

  if (ctx->polygon.front == GL_FILL) {
    GLint sx = 0, sy = 0;
    sx = (point[0] + point[4] + point[8] + point[12]) / 4;
    sy = (point[1] + point[5] + point[9] + point[13]) / 4;
    _flood_fill(buf, sx, sy, color);
  }
}

void _sgl_primitive_assembly(void)
{
  //GET_CURRENT_CONTEXT(ctx);
  
}

void _sgl_pipeline_rasterize(void)
{
  GET_CURRENT_CONTEXT(ctx);
  struct sgl_renderbuffer* cbf = &ctx->drawbuffer->color_buffer;

  GLint i = 0, j = 0;
  GLfloat point[16], color[16], normal[16];
  GLenum prim_type = ctx->render_state.current_exec_primitive;
  GLint n_data = (prim_type == GL_POINTS) * 1 +
                 (prim_type == GL_LINES) * 2 +
                 (prim_type == GL_TRIANGLES) * 3 +
                 (prim_type == GL_QUADS) * 4;

  for (i = 0; i < ctx->vector_point.count / n_data; ++i) {
    for (j = 0; j < n_data; ++j) {
      memcpy(point+j*4, VEC_ELT(&ctx->vector_point, GLvoid, i*n_data + j),
             4 * sizeof(GLfloat));
      memcpy(normal+j*4, VEC_ELT(&ctx->vector_normal, GLvoid, i*n_data + j),
             4 * sizeof(GLfloat));
      memcpy(color+j*4, VEC_ELT(&ctx->vector_color, GLvoid, i*n_data + j),
             4 * sizeof(GLfloat));
    }

    switch (prim_type) {
    case GL_POINTS:
      _sgl_draw_point(cbf, point, color);
      break;
    case GL_LINES:
      _sgl_draw_line(cbf, point, color);
      break;
    case GL_TRIANGLES:
      _sgl_draw_triangle(cbf, point, color);
      break;
    case GL_QUADS:
      _sgl_draw_quads(cbf, point, color);
      break;
    }
  }
}

void glPolygonMode(GLenum face, GLenum mode)
{
  GET_CURRENT_CONTEXT(ctx);

  if (mode != GL_POINT && mode != GL_LINE && mode != GL_FILL) {
    _sgl_error(ctx, GL_INVALID_ENUM, "glPolygonMode(): Invalid polygon mode\n");
    return;
  }

  switch (face) {
  case GL_FRONT:
    ctx->polygon.front = mode;
    break;
  case GL_BACK:
    ctx->polygon.back = mode;
    break;
  case GL_FRONT_AND_BACK:
    ctx->polygon.front = ctx->polygon.back = mode;
    break;
  default:
    _sgl_error(ctx, GL_INVALID_ENUM, "glPolygonMode(): Invalid face\n");
  }
}
