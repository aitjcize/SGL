#include "rasterize.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logging.h"
#include "macros.h"
#include "macros.h"
#include "types.h"
#include "math/m_matrix.h"
#include "math/m_vector.h"

void _sgl_render_pixel(struct sgl_framebuffer* buf,
                       GLuint x, GLuint y, GLuint z, GLuint cc)
{
  GET_CURRENT_CONTEXT(ctx);
  if (x > 0 && y >= 0 && x <= buf->width && y < buf->height) {
    BUF_SET_C(buf->r_color_buf, x, y, cc);
    BUF_SET_D(buf->r_depth_buf, x, y, NORMALIZE_Z(ctx, z));
  }
}

void _flood_fill(struct sgl_framebuffer* buf, GLint sx, GLint sy, GLfloat sz,
                 GLuint cc)
{
  GET_CURRENT_CONTEXT(ctx);
  if (sx < 0 || sy < 0 || sx > buf->width || sy > buf->height)
    return;

  GLuint data[4];

  data[0] = sx; data[1] = sy; data[2] = sz; data[3] = cc;

  _math_vector4f_push_back(&ctx->flood_fill, (GLfloat*)data, 3);
  while (_math_vector4f_pop_back(&ctx->flood_fill, (GLfloat*)data, 3)) {
    sx = data[0]; sy = data[1]; sz = data[2]; cc = data[3];

    _sgl_render_pixel(buf, sx, sy, sz, cc);
    data[3] = cc;
    if (sx + 1 <= buf->width && BUF_GET_C(buf->r_color_buf, sx + 1, sy) != cc) {
      data[0] = sx + 1; data[1] = sy; data[2] = sz;
      _math_vector4f_push_back(&ctx->flood_fill, (GLfloat*)data, 3);
    }

    if (sx - 1 > 0 && BUF_GET_C(buf->r_color_buf, sx - 1, sy) != cc) {
      data[0] = sx - 1; data[1] = sy; data[2] = sz;
      _math_vector4f_push_back(&ctx->flood_fill, (GLfloat*)data, 3);
    }

    if (sy + 1 < buf->height && BUF_GET_C(buf->r_color_buf, sx, sy + 1) != cc) {
      data[0] = sx; data[1] = sy + 1; data[2] = sz;
      _math_vector4f_push_back(&ctx->flood_fill, (GLfloat*)data, 3);
    }

    if (sy - 1 >= 0 && BUF_GET_C(buf->r_color_buf, sx, sy - 1) != cc) {
      data[0] = sx; data[1] = sy - 1; data[2] = sz;
      _math_vector4f_push_back(&ctx->flood_fill, (GLfloat*)data, 3);
    }
  }
  _math_vector4f_lazy_free(&ctx->flood_fill);
}

void _sgl_draw_point(struct sgl_framebuffer* buf, GLfloat* point,
                     GLfloat* color)
{
  _sgl_render_pixel(buf, point[0], point[1], point[2], COLOR_FF(color));
}

void _sgl_draw_line(struct sgl_framebuffer* buf,
                    GLfloat* point1, GLfloat* point2,
                    GLfloat* color1, GLfloat* color2)
{
  GLint x1 = point1[0], y1 = point1[1], z1 = point1[2],
        x2 = point2[0], y2 = point2[1], z2 = point2[2];
  /* Bresenham's Algorithm */
  int dx, dy, i, err;
  int incx, incy, incz, inc1, inc2;
  int x = x1; 
  int y = y1;
  int z = z1;
  int cc = COLOR_FF(color1);

  dx = abs(x2 - x1);
  dy = abs(y2 - y1);

  incx = (x2 > x1) - (x2 < x1);
  incy = (y2 > y1) - (y2 < y1);
  incz = (z2 > z1) - (z2 < z1);

  /* Draw m <= 1 Line */
  if(dx > dy) {
    _sgl_render_pixel(buf, x, y, z, cc);
    err = (2 * dy) - dx;
    inc1 = 2 * (dy - dx);
    inc2 = 2 * dy;

    for(i = 0; i < dx; ++i) {
      if(err >= 0) {
        y += incy;
        err += inc1;
      }
      else
        err += inc2;
      x += incx;
      _sgl_render_pixel(buf, x, y, z, cc);
    }
  } else {
    _sgl_render_pixel(buf, x, y, z, cc);
    err = (2 * dx) - dy;
    inc1 = 2 * (dx - dy);
    inc2 = 2 * dx;

    for(i = 0; i < dy; ++i) {
      if(err >= 0) {
        x += incx;
        err += inc1;
      }
      else
        err += inc2;

      y += incy;
      _sgl_render_pixel(buf, x, y, z, cc);
    }
  }
}

void _sgl_draw_triangle(struct sgl_framebuffer* buf,
                        GLfloat* p1, GLfloat* p2, GLfloat* p3,
                        GLfloat* c1, GLfloat* c2, GLfloat* c3)
{
  GET_CURRENT_CONTEXT(ctx);

  _sgl_draw_line(buf, p1, p2, c1, c2);
  _sgl_draw_line(buf, p2, p3, c2, c3);
  _sgl_draw_line(buf, p3, p1, c3, c1);

  if (ctx->polygon.front == GL_FILL) {
    GLint sx = 0, sy = 0, sz = 0;
    sx = (p1[0] + p2[0] + p3[0]) / 3;
    sy = (p1[1] + p2[1] + p3[1]) / 3;
    sz = (p1[2] + p2[2] + p3[2]) / 3;
    _flood_fill(buf, sx, sy, sz, COLOR_FF(c1));
  }
}

void _sgl_draw_triangle_v(struct sgl_framebuffer* buf, GLfloat* point,
                          GLfloat* color)
{
  _sgl_draw_triangle(buf, &point[0], &point[4], &point[8],
                          &color[0], &color[4], &color[8]);
}

void _sgl_draw_triangle_strip(struct sgl_framebuffer* buf, GLfloat* point,
                              GLfloat* color, GLuint count)
{
  GET_CURRENT_CONTEXT(ctx);
  static GLfloat prev_point[8], prev_color[8];

  if (count == 0) {
    MOVE_FLOAT_4(prev_point, point);
    MOVE_FLOAT_4(prev_color, color);
  } else if (count == 1) {
    MOVE_FLOAT_4(prev_point + 4, point);
    MOVE_FLOAT_4(prev_color + 4, color);
    _sgl_draw_line(buf, prev_point, prev_point + 4, prev_color, prev_color + 4);
  } else {
    _sgl_draw_line(buf, prev_point, point, prev_color, color);
    _sgl_draw_line(buf, prev_point + 4, point, prev_color + 4, color);

    if (ctx->polygon.front == GL_FILL) {
      GLint sx = 0, sy = 0, sz = 0;
      sx = (prev_point[0] + prev_point[4] + point[0]) / 3;
      sy = (prev_point[1] + prev_point[5] + point[1]) / 3;
      sz = (prev_point[2] + prev_point[6] + point[2]) / 3;
      _flood_fill(buf, sx, sy, sz, COLOR_FF(color));
    }

    MOVE_FLOAT_4(prev_point, prev_point + 4);
    MOVE_FLOAT_4(prev_color, prev_color + 4);
    MOVE_FLOAT_4(prev_point + 4, point);
    MOVE_FLOAT_4(prev_color + 4, color);
  }
}

void _sgl_draw_quad(struct sgl_framebuffer* buf,
                    GLfloat* p1, GLfloat* p2, GLfloat* p3, GLfloat* p4,
                    GLfloat* c1, GLfloat* c2, GLfloat* c3, GLfloat* c4)
{
  GET_CURRENT_CONTEXT(ctx);
  _sgl_draw_line(buf, p1, p2, c1, c2);
  _sgl_draw_line(buf, p2, p3, c2, c3);
  _sgl_draw_line(buf, p3, p4, c3, c4);
  _sgl_draw_line(buf, p4, p1, c4, c1);

  if (ctx->polygon.front == GL_FILL) {
    GLint sx = 0, sy = 0, sz = 0;
    sx = (p1[0] + p2[0] + p3[0] + p4[0]) / 4;
    sy = (p1[1] + p2[1] + p3[1] + p4[1]) / 4;
    sz = (p1[2] + p2[2] + p3[2] + p4[2]) / 4;
    _flood_fill(buf, sx, sy, sz, COLOR_FF(c1));
  }
}

void _sgl_draw_quad_v(struct sgl_framebuffer* buf, GLfloat* point,
                      GLfloat* color)
{
  _sgl_draw_quad(buf, &point[0], &point[4], &point[8], &point[12],
                      &color[0], &color[4], &color[8], &color[12]);
}

void _sgl_primitive_assembly(void)
{
  //GET_CURRENT_CONTEXT(ctx);
  
}

void _sgl_pipeline_draw_list(void)
{
  GET_CURRENT_CONTEXT(ctx);
  GLint i = 0, j = 0, idx = 0;
  GLfloat *point, *color, *normal;
  GLenum prim_mode = ctx->render_state.current_exec_primitive;
  GLint n_data = (prim_mode == GL_POINTS) * 1 +
                 (prim_mode == GL_LINES) * 2 +
                 (prim_mode == GL_TRIANGLES) * 3 +
                 (prim_mode == GL_TRIANGLE_STRIP) * 1 +
                 (prim_mode == GL_QUADS) * 4;

  for (i = 0; i < ctx->vector_point.count / n_data; ++i) {
    idx = i * n_data;
    point = VEC_ELT(&ctx->vector_point, GLvoid, idx);
    color = VEC_ELT(&ctx->vector_color, GLvoid, idx);
    normal = VEC_ELT(&ctx->vector_normal, GLvoid, idx);

    switch (prim_mode) {
    case GL_POINTS:
      _sgl_draw_point(ctx->drawbuffer, point, color);
      break;
    case GL_LINES:
      _sgl_draw_line(ctx->drawbuffer, point, point + 4, color, color + 4);
      break;
    case GL_TRIANGLES:
      _sgl_draw_triangle_v(ctx->drawbuffer, point, color);
      break;
    case GL_TRIANGLE_STRIP:
      _sgl_draw_triangle_strip(ctx->drawbuffer, point, color, i);
      break;
    case GL_QUADS:
      _sgl_draw_quad_v(ctx->drawbuffer, point, color);
      break;
    }
    if (ctx->depth.test)
      _sgl_pipeline_depth_test();
  }
}

void _sgl_pipeline_draw_array(void)
{
#if 0
  GET_CURRENT_CONTEXT(ctx);
  GLint i = 0, j = 0, idx = 0;
  GLfloat *point, *color, *normal;
  char* pidx = (char*)ctx->varray.indices_ptr;
  GLint ts = _sgl_sizeof_type(ctx->varray.type);
  GLenum prim_mode = ctx->render_state.current_exec_primitive;
  GLint n_data = (prim_mode == GL_POINTS) * 1 +
                 (prim_mode == GL_LINES) * 2 +
                 (prim_mode == GL_TRIANGLES) * 3 +
                 (prim_mode == GL_TRIANGLE_STRIP) * 1 +
                 (prim_mode == GL_QUADS) * 4;

  point = VEC_ELT(&ctx->vector_point, GLvoid, idx);
  color = VEC_ELT(&ctx->vector_color, GLvoid, idx);
  normal = VEC_ELT(&ctx->vector_normal, GLvoid, idx);

  switch (prim_mode) {
  case GL_POINTS:
    for (i = 0; i < ctx->vector_point.count / n_data; ++i) {
      idx = i * n_data;
      _sgl_draw_point(ctx->drawbuffer,
                      VEC_ELT(&ctx->vector_point, GLvoid, idx),
                      VEC_ELT(&ctx->vector_color, GLvoid, idx));
      if (ctx->depth.test)
        _sgl_pipeline_depth_test();
    }
    break;
  case GL_LINES:
    for (i = 0; i < ctx->vector_point.count / n_data; ++i, pidx += ts * 2) {
      _sgl_draw_point(ctx->drawbuffer,
                      VEC_ELT(&ctx->vector_point, GLvoid, pidx + ts * 1),
                      VEC_ELT(&ctx->vector_point, GLvoid, pidx + ts * 2),
                      VEC_ELT(&ctx->vector_color, GLvoid, pidx + ts * 1),
                      VEC_ELT(&ctx->vector_color, GLvoid, pidx + ts * 2));
      if (ctx->depth.test)
        _sgl_pipeline_depth_test();
    }
    break;
  case GL_TRIANGLES:
    for (i = 0; i < ctx->vector_point.count / n_data; ++i, pidx += ts * 3) {
      _sgl_draw_triangle_v(ctx->drawbuffer,
                           VEC_ELT(&ctx->vector_point, GLvoid, pidx + ts * 1),
                           VEC_ELT(&ctx->vector_point, GLvoid, pidx + ts * 2),
                           VEC_ELT(&ctx->vector_point, GLvoid, pidx + ts * 3),
                           VEC_ELT(&ctx->vector_color, GLvoid, pidx + ts * 1),
                           VEC_ELT(&ctx->vector_color, GLvoid, pidx + ts * 2),
                           VEC_ELT(&ctx->vector_color, GLvoid, pidx + ts * 3));
      if (ctx->depth.test)
        _sgl_pipeline_depth_test();
    }
    break;
  case GL_TRIANGLE_STRIP:
    _sgl_draw_triangle_strip(ctx->drawbuffer, point, color, i);
    for (i = 0; i < ctx->vector_point.count / n_data; ++i, pidx += ts) {
      _sgl_draw_triangle_strip(ctx->drawbuffer,
                               VEC_ELT(&ctx->vector_point, GLvoid, pidx + ts),
                               VEC_ELT(&ctx->vector_color, GLvoid, pidx + ts));
      if (ctx->depth.test)
        _sgl_pipeline_depth_test();
    }
    break;
  case GL_QUADS:
    _sgl_draw_quad_v(ctx->drawbuffer, point, color);
    for (i = 0; i < ctx->vector_point.count / n_data; ++i, pidx += ts * 6) {
      _sgl_draw_quad_v(ctx->drawbuffer,
                       VEC_ELT(&ctx->vector_point, GLvoid, pidx + ts * 1),
                       VEC_ELT(&ctx->vector_point, GLvoid, pidx + ts * 2),
                       VEC_ELT(&ctx->vector_point, GLvoid, pidx + ts * 3),
                       VEC_ELT(&ctx->vector_point, GLvoid, pidx + ts * 4),
                       VEC_ELT(&ctx->vector_color, GLvoid, pidx + ts * 1),
                       VEC_ELT(&ctx->vector_color, GLvoid, pidx + ts * 2),
                       VEC_ELT(&ctx->vector_color, GLvoid, pidx + ts * 3),
                       VEC_ELT(&ctx->vector_color, GLvoid, pidx + ts * 4));
      if (ctx->depth.test)
        _sgl_pipeline_depth_test();
    }
    break;
  }
#endif
}

void _sgl_pipeline_rasterize(void)
{
  GET_CURRENT_CONTEXT(ctx);
  if (ctx->render_state.type == 0)
    _sgl_pipeline_draw_list();
  else if (ctx->render_state.type == GL_VERTEX_ARRAY)
    _sgl_pipeline_draw_array();
}

