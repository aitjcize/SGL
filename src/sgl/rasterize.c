#include "rasterize.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logging.h"
#include "macros.h"
#include "types.h"
#include "math/m_matrix.h"
#include "math/m_vector.h"

void _sgl_render_pixel(struct sgl_framebuffer* buf,
                       GLuint x, GLuint y, GLuint z, GLuint cc)
{
  if (x > 0 && y >= 0 && x <= buf->width && y < buf->height) {
    ((GLuint*)buf->color_buf.data)[(buf->height-y-1)*buf->width+x-1] = cc;
    ((GLfloat*)buf->depth_buf.data)[(buf->height-y-1)*buf->width+x-1] = z;
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
    if (sx + 1 <= buf->width && BUF_GET_C(buf, sx + 1, sy) != cc) {
      data[0] = sx + 1; data[1] = sy; data[2] = sz;
      _math_vector4f_push_back(&ctx->flood_fill, (GLfloat*)data, 3);
    }

    if (sx - 1 > 0 && BUF_GET_C(buf, sx - 1, sy) != cc) {
      data[0] = sx - 1; data[1] = sy; data[2] = sz;
      _math_vector4f_push_back(&ctx->flood_fill, (GLfloat*)data, 3);
    }

    if (sy + 1 < buf->height && BUF_GET_C(buf, sx, sy + 1) != cc) {
      data[0] = sx; data[1] = sy + 1; data[2] = sz;
      _math_vector4f_push_back(&ctx->flood_fill, (GLfloat*)data, 3);
    }

    if (sy - 1 >= 0 && BUF_GET_C(buf, sx, sy - 1) != cc) {
      data[0] = sx; data[1] = sy - 1; data[2] = sz;
      _math_vector4f_push_back(&ctx->flood_fill, (GLfloat*)data, 3);
    }
  }
  _math_vector4f_lazy_free(&ctx->flood_fill);
}

void _sgl_raster_point(struct sgl_framebuffer* buf, GLfloat* point,
                       GLfloat* color)
{
  _sgl_render_pixel(buf, point[0], point[1], point[2], COLOR_FF(color));
}

void _sgl_raster_line(struct sgl_framebuffer* buf, GLfloat* point,
                      GLfloat* color)
{
  GLint x1 = point[0], y1 = point[1], z1 = point[2],
        x2 = point[4], y2 = point[5], z2 = point[6];

  /* Bresenham's Algorithm */
  int dx, dy, i, err;
  int incx, incy, incz, inc1, inc2;
  int x = x1; 
  int y = y1;
  int z = z1;
  int cc = COLOR_FF(color);

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

void _sgl_raster_triangle(struct sgl_framebuffer* buf, GLfloat* point,
                          GLfloat* color)
{
  GET_CURRENT_CONTEXT(ctx);
  GLfloat wrap_point[8];
  memcpy(wrap_point, point + 8, sizeof(GLfloat) * 4);
  memcpy(wrap_point + 4, point, sizeof(GLfloat) * 4);

  _sgl_raster_line(buf, point, color);
  _sgl_raster_line(buf, point + 4, color);
  _sgl_raster_line(buf, wrap_point, color);

  if (ctx->polygon.front == GL_FILL) {
    GLint sx = 0, sy = 0, sz = 0;
    sx = (point[0] + point[4] + point[8]) / 3;
    sy = (point[1] + point[5] + point[9]) / 3;
    sz = (point[2] + point[6] + point[10]) / 3;
    _flood_fill(buf, sx, sy, sz, COLOR_FF(color));
  }
}

void _sgl_raster_quads(struct sgl_framebuffer* buf, GLfloat* point,
                       GLfloat* color)
{
  GET_CURRENT_CONTEXT(ctx);
  GLfloat wrap_point[8];
  memcpy(wrap_point, point + 12, sizeof(GLfloat) * 4);
  memcpy(wrap_point + 4, point, sizeof(GLfloat) * 4);

  _sgl_raster_line(buf, point, color);
  _sgl_raster_line(buf, point + 4, color);
  _sgl_raster_line(buf, point + 8, color);
  _sgl_raster_line(buf, wrap_point, color);

  if (ctx->polygon.front == GL_FILL) {
    GLint sx = 0, sy = 0, sz = 0;
    sx = (point[0] + point[4] + point[8] + point[12]) / 4;
    sy = (point[1] + point[5] + point[9] + point[13]) / 4;
    sz = (point[2] + point[6] + point[10] + point[14]) / 4;
    _flood_fill(buf, sx, sy, sz, COLOR_FF(color));
  }
}

void _sgl_primitive_assembly(void)
{
  //GET_CURRENT_CONTEXT(ctx);
  
}

void _sgl_pipeline_draw_list(void)
{
  GET_CURRENT_CONTEXT(ctx);
  GLint i = 0, j = 0, idx = 0, size = 4 * sizeof(GLfloat);
  GLfloat point[16], color[16], normal[16];
  GLenum prim_mode = ctx->render_state.current_exec_primitive;
  GLint n_data = (prim_mode == GL_POINTS) * 1 +
                 (prim_mode == GL_LINES) * 2 +
                 (prim_mode == GL_TRIANGLES) * 3 +
                 (prim_mode == GL_QUADS) * 4;

  for (i = 0; i < ctx->vector_point.count / n_data; ++i) {
    for (j = 0; j < n_data; ++j) {
      idx = i * n_data + j;
      memcpy(point + j*4, VEC_ELT(&ctx->vector_point, GLvoid, idx), size);
      memcpy(normal + j*4, VEC_ELT(&ctx->vector_normal, GLvoid, idx), size);
      memcpy(color + j*4, VEC_ELT(&ctx->vector_color, GLvoid, idx), size);
    }

    switch (prim_mode) {
    case GL_POINTS:
      _sgl_raster_point(ctx->drawbuffer, point, color);
      break;
    case GL_LINES:
      _sgl_raster_line(ctx->drawbuffer, point, color);
      break;
    case GL_TRIANGLES:
      _sgl_raster_triangle(ctx->drawbuffer, point, color);
      break;
    case GL_QUADS:
      _sgl_raster_quads(ctx->drawbuffer, point, color);
      break;
    }
  }
}

void _sgl_pipeline_draw_array(void)
{
  GET_CURRENT_CONTEXT(ctx);
  GLint i = 0, j = 0, k = 0, idx = 0;

  GLfloat point[16], color[16], normal[16];
  GLenum prim_mode = ctx->varray.mode;
  GLint n_data = (prim_mode == GL_POINTS) * 1 +
                 (prim_mode == GL_LINES) * 2 +
                 (prim_mode == GL_TRIANGLES) * 3 +
                 (prim_mode == GL_QUADS) * 4;

  for (i = 0; i < 16; ++i)
    color[i] = 1;

  for (i = 0; i < ctx->varray.count; ++i) {
    for (j = 0; j < n_data; ++j) {
      idx = *((GLshort*)ctx->varray.indices_ptr + i * n_data + j);
      for (k = 0; k < ctx->vertex_pointer.size; ++k)
        point[j * 4 + k] = *VEC_ELT(&ctx->vertex_pointer, GLfloat, idx + k);
      _sgl_affine_transform(&ctx->model_projection_matrix, point + j*n_data);
    }

    switch (prim_mode) {
    case GL_POINTS:
      _sgl_raster_point(ctx->drawbuffer, point, color);
      break;
    case GL_LINES:
      _sgl_raster_line(ctx->drawbuffer, point, color);
      break;
    case GL_TRIANGLES:
      _sgl_raster_triangle(ctx->drawbuffer, point, color);
      break;
    case GL_QUADS:
      _sgl_raster_quads(ctx->drawbuffer, point, color);
      break;
    }
  }
}

void _sgl_pipeline_rasterize(void)
{
  GET_CURRENT_CONTEXT(ctx);
  if (ctx->render_state.type == 0)
    _sgl_pipeline_draw_list();
  else if (ctx->render_state.type == GL_VERTEX_ARRAY)
    _sgl_pipeline_draw_array();
}

