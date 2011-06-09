#include "rasterize.h"

#include <stdio.h>

#include "types.h"
#include "math/m_matrix.h"
#include "math/m_vector.h"

#define BUF_P(buf, type, x, y) \
  ((type*)buf->data)[(buf->height - (y))*buf->width+(x)]

void _sgl_draw_point(struct sgl_renderbuffer* buf, GLfloat* point,
                     GLfloat* color)
{
  GET_CURRENT_CONTEXT(ctx);
  int data = 0xffffffff;
  printf("draw: %f, %f\n", point[0], point[1]);
  BUF_P(buf, GLuint, (GLint)point[0], (GLint)point[1]) = data;
  
}

void _sgl_draw_line(struct sgl_renderbuffer* buf, GLfloat* point,
                    GLfloat* color)
{
#if 0
  /* Bresenham's Algorithm */
  int dx, dy, i, err;
  int incx, incy, inc1, inc2;
  int x = x1; 
  int	y = y1;

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
    //draw_pixel(x, y, BLACK);
    err = (2 * dy) - dx;
    inc1 = 2 * (dy - dx);
    inc2 = 2 * dy;

    for(i = 0 ; i < dx ; i++)
    {
      if(err >= 0)
      {
        y += incy;
        err += inc1;
      }
      else
        err += inc2;

      x += incx;
      //draw_pixel(x, y, BLACK);
    }
  }
  else
  {
    //draw_pixel(x, y, BLACK);
    err = (2 * dx) - dy;
    inc1 = 2 * (dx - dy);
    inc2 = 2 * dx;

    for(i = 0 ; i < dy ; i++)
    {
      if(err >= 0)
      {
        x += incx;
        err += inc1;
      }
      else
        err += inc2;

      y += incy;
      //draw_pixel(x, y, BLACK);
    }
  }
#endif
}

void _sgl_draw_triangle(struct sgl_renderbuffer* buf, GLfloat* point,
                        GLfloat* color)
{
  
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

  GLint i = 0;
  GLfloat point[12], color[12], normal[12];
  GLint n_data = (ctx->render_state.current_exec_primitive == GL_POINTS) * 1 +
                 (ctx->render_state.current_exec_primitive == GL_LINES) * 2 +
                 (ctx->render_state.current_exec_primitive == GL_TRIANGLES) * 3;

  //_math_vector4f_print(&ctx->vector_point);
  while (!_math_vector4f_empty(&ctx->vector_point)) {
    for (i = 0; i < n_data; ++i) {
      _math_vector4f_pop_back(&ctx->vector_point, point + i * 4, 4);
      _math_vector4f_pop_back(&ctx->vector_color, color + i * 4, 4);
      _math_vector4f_pop_back(&ctx->vector_normal, normal + i * 4, 4);
    }

    switch (ctx->render_state.current_exec_primitive) {
      case GL_POINTS:
        _sgl_draw_point(cbf, point, color);
        break;
      case GL_LINES:
        _sgl_draw_line(cbf, point, color);
        break;
      case GL_TRIANGLES:
        _sgl_draw_triangle(cbf, point, color);
        break;
    }
  }

}
