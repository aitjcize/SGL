/**
 * @file   rasterize.c
 *
 * Copyright (C) 2011 - SGL Authors <aitjcize@gmail.com>
 * All Rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "rasterize.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "framebuffer.h"
#include "logging.h"
#include "macros.h"
#include "macros.h"
#include "types.h"
#include "math/m_matrix.h"
#include "math/m_vector.h"

static GLfloat _ll_first_point[4], _ll_first_color[4];
static GLfloat _ll_prev_point[4], _ll_prev_color[4];

static GLint _ts_count;
static GLfloat _ts_prev_point[8], _ts_prev_color[8];

/**
 * @brief Insert an edge to the edge table
 * @param x x component
 * @param y y component
 */
void _insert_edge(GLint x, GLint y)
{
  GET_CURRENT_CONTEXT(ctx);
  GLint* et = ctx->drawbuffer->edge_tab;
  GLint cidx = EDGE_TABLE_SIZE -1;
  GLint oidx = EDGE_TABLE_SIZE -2;
  GLuint count = ET_GET(et, y, cidx);
  GLint l = 0;

  if (count >= EDGE_TABLE_SIZE - 2)
    return;
  for (l = 0; l < count; ++l)
    if (ET_GET(et, y, l) == x)
      return;

  switch (count % 2) {
    case 0:
      if (count && abs(ET_GET(et, y, count -1) -x) == ET_GET(et, y, oidx))
        ++ET_GET(et, y, oidx);
      else {
        ET_GET(et, y, oidx) = 1;
        ET_GET(et, y, count) = x;
        ++ET_GET(et, y, cidx);
      }
      break;
    case 1:
      if (abs(ET_GET(et, y, count -1) -x) == 1) {
        ET_GET(et, y, count -1) = x;
      } else {
        ET_GET(et, y, count) = x;
        ++ET_GET(et, y, cidx);
      }
      break;
  }
}

/**
 * @brief Perfrom scanline fill given a edge table
 * @param buf a struct sgl_framebuffer
 */
void _scanline_fill(struct sgl_framebuffer* buf, GLfloat* point, GLfloat* color)
{
  GET_CURRENT_CONTEXT(ctx);

  /* Find AABB */
  GLint xmin = INT_MAX, ymin = INT_MAX, xmax = INT_MIN, ymax = INT_MIN;
  GLint i = 0;
  for (i = 0; i < 3; ++i) {
    if (point[i << 2] < xmin) {
      xmin = point[i << 2];
    }
    if (point[i << 2] > xmax) {
      xmax = point[i << 2];
    }
    if (point[(i << 2) + 1] < ymin) {
      ymin = point[(i << 2) + 1];
    }
    if (point[(i << 2) + 1] > ymax) {
      ymax = point[(i << 2) + 1];
    }
  }

  /* Find three edge equation */
  GLfloat m1, m2, m3, b1, b2, b3;

  m1 = (point[5] - point[1]) / (point[4] - point[0]);
  b1 = point[1] - m1 * point[0];

  m2 = (point[9] - point[5]) / (point[8] - point[4]);
  b2 = point[5] - m2 * point[4];

  m3 = (point[1] - point[9]) / (point[0] - point[8]);
  b3 = point[9] - m3 * point[8];

  GLint x = 0, y = 0, d = 0, dx = 0, start = 0, end = 0;
  GLfloat a0 = 0, b0 = 0, c0 = 0, a = 0, b = 0, c = 0, z = 0;
  color_t cc;

#define DISTANCE(m, b, x, y, ox) (isinf(m)? (x - ox): (m * (x) + b - (y)))

  a0 = DISTANCE(m1, b1, point[8], point[9], point[0]);
  b0 = DISTANCE(m2, b2, point[0], point[1], point[4]);
  c0 = DISTANCE(m3, b3, point[4], point[5], point[8]);

  GLint* et = buf->edge_tab;

  for (y = 0; y < buf->height; ++y) {
    for (x = 0; x + 1 < ET_GET(et, y, EDGE_TABLE_SIZE -1); x += 2) {
      start = ET_GET(et, y, x);
      end = ET_GET(et, y, x + 1);

      dx = (end > start)? 1: -1;
      for (d = start + dx; d != end; d += dx) {
        a = DISTANCE(m1, b1, d, y, point[0]) / a0;
        b = DISTANCE(m2, b2, d, y, point[4]) / b0;
        c = DISTANCE(m3, b3, d, y, point[8]) / c0;

        z = NORMALIZE_Z(ctx,DEPTH_WSUM(a, point[10], b, point[2], c, point[6]));

        if (ctx->depth.test && z > BUF_GET_D(&buf->depth_buf, d, y))
          continue;


        cc = COLOR_WSUM(a, COLOR_FF_CT(color + 8),
                        b, COLOR_FF_CT(color + 0),
                        c, COLOR_FF_CT(color + 4));
        BUF_SET_C(&buf->color_buf, d, y, cc.val);
        BUF_SET_D(&buf->depth_buf, d, y, z);
      }
    }
  }
  _sgl_framebuffer_edge_table_clear();

#undef DISTANCE
}

/**
 * @brief Render a pixel and if GL_DEPTH_TEST is enabled, perform depth test.
 * @param buf a struct sgl_framebuffer
 * @param x x component
 * @param y y component
 * @param z z component
 * @param cc color component
 */
void _sgl_render_pixel(struct sgl_framebuffer* buf,
                       GLint x, GLint y, GLint z, GLint cc)
{
  GET_CURRENT_CONTEXT(ctx);
  GLfloat fz = NORMALIZE_Z(ctx, z);

  x = CLAMP(x, 1, buf->width);
  y = CLAMP(y, 0, buf->height -1);

  BUF_SET_C(&buf->t_color_buf, x, y, cc);
  BUF_SET_D(&buf->t_depth_buf, x, y, fz);

  if (ctx->polygon.front == GL_FILL)
    _insert_edge(x, y);

  if ((ctx->depth.test && fz > BUF_GET_D(&buf->depth_buf, x, y)))
    return;

  BUF_SET_C(&buf->color_buf, x, y, cc);
  BUF_SET_D(&buf->depth_buf, x, y, fz);
}

/**
 * @brief Draw a single point
 * @param buf a struct sgl_framebuffer
 * @param point point (x, y, z) specified by a float array
 * @param color color (r, g, b, a) specified by a float array
 */
void _sgl_draw_point(struct sgl_framebuffer* buf, GLfloat* point,
                     GLfloat* color)
{
  _sgl_render_pixel(buf, point[0], point[1], point[2], COLOR_FF(color));
}

/**
 * @brief Draw a line
 * @param buf a struct sgl_framebuffer
 * @param point1 first point (x, y, z) specified by a float array
 * @param point2 second point (x, y, z) specified by a float array
 * @param color1 first color (r, g, b, a) specified by a float array
 * @param color2 second color (r, g, b, a) specified by a float array
 *
 * Draw a line between point1 and point2, if glShadeModel is GL_SMOOTH, color
 * between these two point is interpolated with color1 and color2
 */
void _sgl_draw_line(struct sgl_framebuffer* buf,
                    GLfloat* point1, GLfloat* point2,
                    GLfloat* color1, GLfloat* color2)
{
  /* Bresenham's Algorithm */

  GLint x1 = point1[0], y1 = point1[1], z1 = point1[2],
        x2 = point2[0], y2 = point2[1], z2 = point2[2];

  GLint dx, dy, i, err;
  GLint incx, incy, inc1, inc2;
  GLfloat a = LEN_2(x2 - x1, y2 - y1), x = x1, y = y1;
  color_t cc1 = COLOR_FF_CT(color1);
  color_t cc2 = COLOR_FF_CT(color2);

  dx = abs(x2 - x1);
  dy = abs(y2 - y1);

  incx = (x2 > x1) - (x2 < x1);
  incy = (y2 > y1) - (y2 < y1);

  /* Draw m <= 1 Line */
  if(dx > dy) {
    _sgl_render_pixel(buf, x, y, LINEAR_IP(z1, z2, LEN_2(x-x1, y-y1), a),
                      COLOR_IP(cc1, cc2, LEN_2(x-x1, y-y1), a));
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
      _sgl_render_pixel(buf, x, y, LINEAR_IP(z1, z2, LEN_2(x-x1, y-y1), a),
                        COLOR_IP(cc1, cc2, LEN_2(x-x1, y-y1), a));
    }
  } else {
    _sgl_render_pixel(buf, x, y, LINEAR_IP(z1, z2, LEN_2(x-x1, y-y1), a),
                      COLOR_IP(cc1, cc2, LEN_2(x-x1, y-y1), a));
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
      _sgl_render_pixel(buf, x, y, LINEAR_IP(z1, z2, LEN_2(x-x1, y-y1), a),
                        COLOR_IP(cc1, cc2, LEN_2(x-x1, y-y1), a));
    }
  }
}

/**
 * @brief Draw a line specified by an varaiable list
 * @param buf a struct sgl_framebuffer
 * @param point a 8 element float array specifing two point
 * @param color a 8 element float array specifing two color
 */
void _sgl_draw_line_v(struct sgl_framebuffer* buf,
                      GLfloat* point, GLfloat* color)
{
  _sgl_draw_line(buf, point, point + 4, color, color + 4);
}

/**
 * @brief Start drawing line loop
 * @param buf a struct sgl_framebuffer
 * @param point point (x, y, z) specified by a float array
 * @param color color (r, g, b, a) specified by a float array
 */
void _sgl_draw_line_loop_start(struct sgl_framebuffer* buf,
                               GLfloat* point, GLfloat* color)
{
  MOVE_FLOAT_4(_ll_first_point, point);
  MOVE_FLOAT_4(_ll_first_color, color);
  MOVE_FLOAT_4(_ll_prev_point, point);
  MOVE_FLOAT_4(_ll_prev_color, color);
}

/**
 * @brief Draw line loop
 * @param buf a struct sgl_framebuffer
 * @param point point (x, y, z) specified by a float array
 * @param color color (r, g, b, a) specified by a float array
 */
void _sgl_draw_line_loop(struct sgl_framebuffer* buf,
                         GLfloat* point, GLfloat* color)
{
  _sgl_draw_line(buf, _ll_prev_point, point, _ll_prev_color, color);
  MOVE_FLOAT_4(_ll_prev_point, point);
  MOVE_FLOAT_4(_ll_prev_color, color);
}

/**
 * @brief Finalize drawing line loop
 * @param buf a struct sgl_framebuffer
 * @param point point (x, y, z) specified by a float array
 * @param color color (r, g, b, a) specified by a float array
 */
void _sgl_draw_line_loop_end(struct sgl_framebuffer* buf,
                             GLfloat* point, GLfloat* color)
{
  _sgl_draw_line(buf, _ll_prev_point, _ll_first_point,
                      _ll_prev_color, _ll_first_color);
}

/**
 * @brief Draw a triangle
 * @param buf a struct sgl_framebuffer
 * @param p1 point (x, y, z) specified by a float array
 * @param p2 point (x, y, z) specified by a float array
 * @param p3 point (x, y, z) specified by a float array
 * @param c1 color (r, g, b, a) specified by a float array
 * @param c2 color (r, g, b, a) specified by a float array
 * @param c3 color (r, g, b, a) specified by a float array
 *
 * Draw a triangle specified by three point: p1, p2, p3, if glShadeModel is
 * GL_SMOOTH, color in between are interpolated.
 */
void _sgl_draw_triangle(struct sgl_framebuffer* buf,
                        GLfloat* p1, GLfloat* p2, GLfloat* p3,
                        GLfloat* c1, GLfloat* c2, GLfloat* c3)
{
  _sgl_draw_line(buf, p1, p2, c1, c2);
  _sgl_draw_line(buf, p2, p3, c2, c3);
  _sgl_draw_line(buf, p3, p1, c3, c1);
}

/**
 * @brief Draw a triangle specified by an varaiable list
 * @param buf a struct sgl_framebuffer
 * @param point a 12 element float array specifing three point
 * @param color a 12 element float array specifing three color
 */
void _sgl_draw_triangle_v(struct sgl_framebuffer* buf, GLfloat* point,
                          GLfloat* color)
{
  GET_CURRENT_CONTEXT(ctx);
  _sgl_draw_triangle(buf, &point[0], &point[4], &point[8],
                          &color[0], &color[4], &color[8]);
  if (ctx->polygon.front == GL_FILL) {
    _scanline_fill(buf, point, color);
  }
}

/**
 * @brief Start drawing triangle strip
 * @param buf a struct sgl_framebuffer
 * @param point a 4 element float array specifing a point
 * @param color a 4 element float array specifing a color
 */
void _sgl_draw_triangle_strip_start(struct sgl_framebuffer* buf, GLfloat* point,
                                    GLfloat* color)
{
  if (_ts_count == 0) {
    MOVE_FLOAT_4(_ts_prev_point, point);
    MOVE_FLOAT_4(_ts_prev_color, color);
  } else if (_ts_count == 1) {
    MOVE_FLOAT_4(_ts_prev_point + 4, point);
    MOVE_FLOAT_4(_ts_prev_color + 4, color);
    GET_CURRENT_CONTEXT(ctx);
    if (ctx->polygon.front != GL_FILL) {
      _sgl_draw_line(buf, _ts_prev_point, _ts_prev_point + 4,
                          _ts_prev_color, _ts_prev_color + 4);
    }
  }
}

/**
 * @brief Draw triangle strip
 * @param buf a struct sgl_framebuffer
 * @param point a 4 element float array specifing a point
 * @param color a 4 element float array specifing a color
 */
void _sgl_draw_triangle_strip(struct sgl_framebuffer* buf, GLfloat* point,
                              GLfloat* color)
{
  GET_CURRENT_CONTEXT(ctx);
  _sgl_draw_line(buf, _ts_prev_point, _ts_prev_point + 4,
                      _ts_prev_color, _ts_prev_color + 4);
  _sgl_draw_line(buf, _ts_prev_point, point, _ts_prev_color, color);
  _sgl_draw_line(buf, _ts_prev_point + 4, point, _ts_prev_color + 4, color);

  if (ctx->polygon.front == GL_FILL) {
    GLfloat t_point[12], t_color[12];
    MOVE_FLOAT_4(t_point, _ts_prev_point);
    MOVE_FLOAT_4(t_point + 4, _ts_prev_point + 4);
    MOVE_FLOAT_4(t_point + 8, point);
    MOVE_FLOAT_4(t_color, _ts_prev_color);
    MOVE_FLOAT_4(t_color + 4, _ts_prev_color + 4);
    MOVE_FLOAT_4(t_color + 8, color);
    _scanline_fill(buf, t_point, t_color);
  }

  MOVE_FLOAT_4(_ts_prev_point, _ts_prev_point + 4);
  MOVE_FLOAT_4(_ts_prev_color, _ts_prev_color + 4);
  MOVE_FLOAT_4(_ts_prev_point + 4, point);
  MOVE_FLOAT_4(_ts_prev_color + 4, color);
}

/**
 * @brief Draw a list of vertices and perform input assembly
 */
void _sgl_pipeline_draw_list(void)
{
  GET_CURRENT_CONTEXT(ctx);
  GLenum prim_mode = ctx->render_state.current_exec_primitive;
  GLint idx = 0, end_idx = 0, offset = 0;
  void (*p_draw_func)(struct sgl_framebuffer* , GLfloat*, GLfloat*);

  end_idx = ctx->vector_point.count;

  /* Pre draw */
  switch (prim_mode) {
  case GL_POINTS:
    offset = 1;
    p_draw_func = _sgl_draw_point;
    break;

  case GL_LINES:
    offset = 2;
    p_draw_func = _sgl_draw_line_v;
    break;

  case GL_LINE_LOOP:
    p_draw_func = _sgl_draw_line_loop;
    _sgl_draw_line_loop_start(ctx->drawbuffer,
                              VEC_ELT(&ctx->vector_point, GLfloat, 0),
                              VEC_ELT(&ctx->vector_color, GLfloat, 0));
    idx = 1;
    end_idx -= 1;
    offset = 1;
    break;

  case GL_TRIANGLES:
    p_draw_func = _sgl_draw_triangle_v;
    offset = 3;
    break;

  case GL_TRIANGLE_STRIP:
    //ctx->render_state.gfill = GL_TRUE;
    p_draw_func = _sgl_draw_triangle_strip;
    _ts_count = 0;
    _sgl_draw_triangle_strip_start(ctx->drawbuffer,
                                   VEC_ELT(&ctx->vector_point, GLfloat, 0),
                                   VEC_ELT(&ctx->vector_color, GLfloat, 0));
    _ts_count = 1;
    _sgl_draw_triangle_strip_start(ctx->drawbuffer,
                                   VEC_ELT(&ctx->vector_point, GLfloat, 1),
                                   VEC_ELT(&ctx->vector_color, GLfloat, 1));
    idx = 2;
    offset = 1;
    break;

  default:
    _sgl_error(ctx, GL_INVALID_ENUM, "_sgl_pipeline_draw_list(): invalid "
               "primitive\n");
    return;
  }

  /* Do draw */
  while (idx < end_idx) {
    p_draw_func(ctx->drawbuffer,
                VEC_ELT(&ctx->vector_point, GLfloat, idx),
                VEC_ELT(&ctx->vector_color, GLfloat, idx));
    idx += offset;
  }

  /* Post draw */
  switch (prim_mode) {
  case GL_LINE_LOOP:
    _sgl_draw_line_loop_end(ctx->drawbuffer,
                            VEC_ELT(&ctx->vector_point, GLfloat, end_idx + 1),
                            VEC_ELT(&ctx->vector_color, GLfloat, end_idx + 1));
    break;
  }
}

/**
 * @brief Rasterization stage in SGL pipeline
 */
void _sgl_pipeline_rasterize(void)
{
  GET_CURRENT_CONTEXT(ctx);
  if (ctx->render_state.type == 0)
    _sgl_pipeline_draw_list();
  //else if (ctx->render_state.type == GL_VERTEX_ARRAY)
  //  _sgl_pipeline_draw_array();
}
