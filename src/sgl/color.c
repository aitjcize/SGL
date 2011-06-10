#include "color.h"

#include "macros.h"

void _sgl_init_color(struct sgl_context* ctx)
{
  ctx->color.clear = 0;

  int i = 0;
  for (i = 0; i < ctx->buffer.width * ctx->buffer.height; ++i)
    *((GLuint*)ctx->drawbuffer->clear_color_buf.data + i) = ctx->color.clear;

  memcpy(ctx->drawbuffer->color_buf.data,
         ctx->drawbuffer->clear_color_buf.data,
         ctx->buffer.width * ctx->buffer.height * sizeof(GLfloat));
  memcpy(ctx->drawbuffer->t_color_buf.data,
         ctx->drawbuffer->clear_color_buf.data,
         ctx->buffer.width * ctx->buffer.height * sizeof(GLfloat));
}


void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->color.clear = COLOR_FE(red, green, blue, alpha);

  int i = 0;
  for (i = 0; i < ctx->buffer.width * ctx->buffer.height; ++i)
    *((GLuint*)ctx->drawbuffer->clear_color_buf.data + i) = ctx->color.clear;
}
