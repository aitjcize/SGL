#include "framebuffer.h"

#include <stdlib.h>

#include "context.h"
#include "renderbuffer.h"
#include "logging.h"

void _sgl_init_framebuffer(struct sgl_context* ctx)
{
  ctx->drawbuffer = malloc(sizeof(struct sgl_framebuffer));
  ctx->drawbuffer->width = ctx->buffer.width;
  ctx->drawbuffer->height = ctx->buffer.height;
  _sgl_init_renderbuffer(&ctx->drawbuffer->color_buffer, GL_UNSIGNED_INT);
  _sgl_init_renderbuffer(&ctx->drawbuffer->depth_buffer, GL_SHORT);
}

void _sgl_free_framebuffer_data(struct sgl_context* ctx)
{
  _sgl_free_renderbuffer(&ctx->drawbuffer->color_buffer);
  _sgl_free_renderbuffer(&ctx->drawbuffer->depth_buffer);
}
