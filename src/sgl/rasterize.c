/**
 * @file   rasterize.c
 *
 * Copyright (C) 2011 - SGL Authors <aitjcize@gmail.com>
 * All Rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "rasterize.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "framebuffer.h"
#include "logging.h"
#include "macros.h"
#include "macros.h"
#include "types.h"
#include "math/m_matrix.h"
#include "math/m_vector.h"

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

void _sgl_render_pixel(struct sgl_framebuffer* buf,
                       GLint x, GLint y, GLint z, GLint cc)
{
  GET_CURRENT_CONTEXT(ctx);
  x = CLAMP(x, 1, buf->width);
  y = CLAMP(y, 0, buf->height -1);
  BUF_SET_C(buf->r_color_buf, x, y, cc);
  BUF_SET_D(buf->r_depth_buf, x, y, NORMALIZE_Z(ctx, z));
  if (ctx->render_state.gfill)
    _insert_edge(x, y);
}

void _scanline_fill(struct sgl_framebuffer* buf)
{
  GLint* et = buf->edge_tab;
  GLint y = 0, x = 0, d = 0, dx = 0, start = 0, end = 0,
        cc1 = 0, cc2 = 0;
  GLfloat z1 = 0, z2 = 0;

  for (y = 0; y < buf->height; ++y) {
    for (x = 0; x + 1 < ET_GET(et, y, EDGE_TABLE_SIZE -1); x += 2) {
      start = ET_GET(et, y, x);
      end = ET_GET(et, y, x + 1);

      dx = (end > start)? 1: -1;
      for (d = start + dx; d != end; d += dx) {
        cc1 = BUF_GET_C(buf->r_color_buf, CLAMP(start, 1, buf->width),
                                         CLAMP(y, 0, buf->height-1));
        cc2 = BUF_GET_C(buf->r_color_buf, CLAMP(end, 1, buf->width),
                                          CLAMP(y, 0, buf->height-1));

        z1 = BUF_GET_D(buf->r_depth_buf, CLAMP(start, 1, buf->width),
                                         CLAMP(y, 0, buf->height-1));
        z2 = BUF_GET_D(buf->r_depth_buf, CLAMP(end, 1, buf->width),
                                         CLAMP(y, 0, buf->height-1));

        BUF_SET_C(buf->r_color_buf, d, y,
                  COLOR_IP(cc1, cc2, abs(d - start), abs(end - start)));
        BUF_SET_D(buf->r_depth_buf, d, y,
                  LINEAR_IP(z1, z2, abs(d - start), abs(end - start)));
      }
    }
  }
  _sgl_framebuffer_edge_table_clear();
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
  /* Bresenham's Algorithm */

  GLint x1 = point1[0], y1 = point1[1], z1 = point1[2],
        x2 = point2[0], y2 = point2[1], z2 = point2[2];

  GLint dx, dy, i, err;
  GLint incx, incy, inc1, inc2;
  GLfloat a = LEN_2(x2 - x1, y2 - y1), x = x1, y = y1;
  GLint cc1 = COLOR_FF(color1);
  GLint cc2 = COLOR_FF(color2);

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

void _sgl_draw_line_loop(struct sgl_framebuffer* buf,
                         GLfloat* point, GLfloat* color,
                         GLint count) 
{
  static GLfloat first_point[4], first_color[4];
  static GLfloat prev_point[4], prev_color[4];

  if (count == 0) {
    MOVE_FLOAT_4(first_point, point);
    MOVE_FLOAT_4(first_color, color);
    MOVE_FLOAT_4(prev_point, point);
    MOVE_FLOAT_4(prev_color, color);
  } else if (count == -1) {
    _sgl_draw_line(buf, prev_point, first_point, prev_color, first_color);
  } else {
    _sgl_draw_line(buf, prev_point, point, prev_color, color);
    MOVE_FLOAT_4(prev_point, point);
    MOVE_FLOAT_4(prev_color, color);
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

  if (ctx->polygon.front == GL_FILL)
    _scanline_fill(buf);
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
    _sgl_draw_line(buf, prev_point, prev_point + 4, prev_color, prev_color + 4);
    _sgl_draw_line(buf, prev_point, point, prev_color, color);
    _sgl_draw_line(buf, prev_point + 4, point, prev_color + 4, color);

    if (ctx->polygon.front == GL_FILL)
      _scanline_fill(buf);

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

  if (ctx->polygon.front == GL_FILL)
    _scanline_fill(buf);
}

void _sgl_draw_quad_v(struct sgl_framebuffer* buf, GLfloat* point,
                      GLfloat* color)
{
  _sgl_draw_quad(buf, &point[0], &point[4], &point[8], &point[12],
                      &color[0], &color[4], &color[8], &color[12]);
}

void _sgl_pipeline_draw_list(void)
{
  GET_CURRENT_CONTEXT(ctx);
  GLint i = 0, idx = 0;
  GLfloat *point, *color;
  GLenum prim_mode = ctx->render_state.current_exec_primitive;
  GLint n_data = (prim_mode == GL_POINTS) * 1 +
                 (prim_mode == GL_LINES) * 2 +
                 (prim_mode == GL_LINE_LOOP) * 1 +
                 (prim_mode == GL_TRIANGLES) * 3 +
                 (prim_mode == GL_TRIANGLE_STRIP) * 1 +
                 (prim_mode == GL_QUADS) * 4;

  /* Move for-loop inside to reduce branching */
  switch (prim_mode) {
  case GL_POINTS:
    ctx->render_state.gfill = GL_FALSE;
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
    ctx->render_state.gfill = GL_FALSE;
    for (i = 0; i < ctx->vector_point.count / n_data; ++i) {
      idx = i * n_data;
      point = VEC_ELT(&ctx->vector_point, GLvoid, idx);
      color = VEC_ELT(&ctx->vector_color, GLvoid, idx);
      _sgl_draw_line(ctx->drawbuffer, point, point + 4, color, color + 4);
      if (ctx->depth.test)
        _sgl_pipeline_depth_test();
    }
    break;
  case GL_LINE_LOOP:
    ctx->render_state.gfill = GL_FALSE;
    for (i = 0; i < ctx->vector_point.count / n_data; ++i) {
      idx = i * n_data;
      _sgl_draw_line_loop(ctx->drawbuffer,
                          VEC_ELT(&ctx->vector_point, GLvoid, idx),
                          VEC_ELT(&ctx->vector_color, GLvoid, idx), i);
    }
    _sgl_draw_line_loop(ctx->drawbuffer, point = 0, color = 0, -1);
    if (ctx->depth.test)
      _sgl_pipeline_depth_test();
    break;
  case GL_TRIANGLES:
    ctx->render_state.gfill = GL_TRUE;
    for (i = 0; i < ctx->vector_point.count / n_data; ++i) {
      idx = i * n_data;
      _sgl_draw_triangle_v(ctx->drawbuffer,
                           VEC_ELT(&ctx->vector_point, GLvoid, idx),
                           VEC_ELT(&ctx->vector_color, GLvoid, idx));
      if (ctx->depth.test)
        _sgl_pipeline_depth_test();
    }
    break;
  case GL_TRIANGLE_STRIP:
    ctx->render_state.gfill = GL_TRUE;
    for (i = 0; i < ctx->vector_point.count / n_data; ++i) {
      idx = i * n_data;
      _sgl_draw_triangle_strip(ctx->drawbuffer,
                               VEC_ELT(&ctx->vector_point, GLvoid, idx),
                               VEC_ELT(&ctx->vector_color, GLvoid, idx), i);
    }
    if (ctx->depth.test)
      _sgl_pipeline_depth_test();
    break;
  case GL_QUADS:
    ctx->render_state.gfill = GL_TRUE;
    for (i = 0; i < ctx->vector_point.count / n_data; ++i) {
      idx = i * n_data;
      _sgl_draw_quad_v(ctx->drawbuffer,
                       VEC_ELT(&ctx->vector_point, GLvoid, idx),
                       VEC_ELT(&ctx->vector_color, GLvoid, idx));
      if (ctx->depth.test)
        _sgl_pipeline_depth_test();
    }
    break;
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

