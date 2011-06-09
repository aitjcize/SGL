#include "rasterize.h"

#include <stdio.h>
#include <string.h>

#include "macros.h"
#include "types.h"
#include "math/m_matrix.h"
#include "math/m_vector.h"

#define BUF_DRAW(buf, x, y, c) \
  ((GLuint*)buf->data)[(buf->height - (y))*buf->width+(x)] = COLOR_FF(c)

void _sgl_draw_point(struct sgl_renderbuffer* buf, GLfloat* point,
                     GLfloat* color)
{
  BUF_DRAW(buf, (GLint)point[0], (GLint)point[1], color);
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
    BUF_DRAW(buf, x, y, color);
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
      BUF_DRAW(buf, x, y, color);
    }
  }
  else
  {
    BUF_DRAW(buf, x, y, color);
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
      BUF_DRAW(buf, x, y, color);
    }
  }
}

void _sgl_draw_triangle(struct sgl_renderbuffer* buf, GLfloat* point,
                        GLfloat* color)
{
  GLfloat wrap_point[8];
  memcpy(wrap_point, point + 8, sizeof(GLfloat) * 4);
  memcpy(wrap_point + 4, point, sizeof(GLfloat) * 4);

  _sgl_draw_line(buf, point, color);
  _sgl_draw_line(buf, point + 4, color);
  _sgl_draw_line(buf, wrap_point, color);
}

void _sgl_draw_quads(struct sgl_renderbuffer* buf, GLfloat* point,
                     GLfloat* color)
{
  GLfloat wrap_point[8];
  memcpy(wrap_point, point + 12, sizeof(GLfloat) * 4);
  memcpy(wrap_point + 4, point, sizeof(GLfloat) * 4);

  _sgl_draw_line(buf, point, color);
  _sgl_draw_line(buf, point + 4, color);
  _sgl_draw_line(buf, point + 8, color);
  _sgl_draw_line(buf, wrap_point, color);
}

void flood_fill(int Seedx, int Seedy)
{
#if 0
  int PixelValue = 0;
  //read_pixel(Seedx, Seedy, &PixelValue);

  if(PixelValue == WHITE)
  {
    //draw_pixel(Seedx, Seedy, BLACK);
    flood_fill(Seedx - 1, Seedy);
    flood_fill(Seedx + 1, Seedy);
    flood_fill(Seedx, Seedy - 1);
    flood_fill(Seedx, Seedy + 1);
  }
#endif
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
