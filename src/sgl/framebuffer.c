#include "framebuffer.h"

#include <stdlib.h>
#include <string.h>

#include "context.h"
#include "renderbuffer.h"
#include "logging.h"

void _sgl_init_framebuffer(struct sgl_context* ctx)
{
  ctx->drawbuffer = malloc(sizeof(struct sgl_framebuffer));
  ctx->drawbuffer->width = ctx->buffer.width;
  ctx->drawbuffer->height = ctx->buffer.height;
  ctx->drawbuffer->depth_max = 65535;
  _sgl_init_renderbuffer(&ctx->drawbuffer->clear_color_buffer, GL_UNSIGNED_INT);
  _sgl_init_renderbuffer(&ctx->drawbuffer->t_color_buffer, GL_UNSIGNED_INT);
  _sgl_init_renderbuffer(&ctx->drawbuffer->t_depth_buffer, GL_SHORT);
  _sgl_init_renderbuffer(&ctx->drawbuffer->t_normal_buffer, GL_UNSIGNED_INT);

  _sgl_init_renderbuffer(&ctx->drawbuffer->color_buffer, GL_UNSIGNED_INT);
  _sgl_init_renderbuffer(&ctx->drawbuffer->depth_buffer, GL_SHORT);
  _sgl_init_renderbuffer(&ctx->drawbuffer->normal_buffer, GL_UNSIGNED_INT);
}

void _sgl_free_framebuffer_data(struct sgl_context* ctx)
{
  _sgl_free_renderbuffer(&ctx->drawbuffer->clear_color_buffer);

  _sgl_free_renderbuffer(&ctx->drawbuffer->t_color_buffer);
  _sgl_free_renderbuffer(&ctx->drawbuffer->t_depth_buffer);
  _sgl_free_renderbuffer(&ctx->drawbuffer->t_normal_buffer);

  _sgl_free_renderbuffer(&ctx->drawbuffer->color_buffer);
  _sgl_free_renderbuffer(&ctx->drawbuffer->depth_buffer);
  _sgl_free_renderbuffer(&ctx->drawbuffer->normal_buffer);
}

void _sgl_clear_framebuffer(GLenum mask)
{
  GET_CURRENT_CONTEXT(ctx);
  int size = ctx->buffer.width * ctx->buffer.height;

  if (mask & GL_COLOR_BUFFER_BIT)
    memcpy(ctx->drawbuffer->color_buffer.data,
           ctx->drawbuffer->clear_color_buffer.data,
           size * sizeof(GLuint));

  if (mask & GL_DEPTH_BUFFER_BIT)
    memset(ctx->drawbuffer->depth_buffer.data, 0, size * sizeof(GLshort));
}
