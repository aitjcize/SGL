#include "framebuffer.h"

#include <stdlib.h>
#include <string.h>

#include "context.h"
#include "renderbuffer.h"
#include "logging.h"

void _sgl_init_framebuffer(struct sgl_context* ctx)
{
  struct sgl_framebuffer* buf = NULL;
  buf = malloc(sizeof(struct sgl_framebuffer));
  buf->width = ctx->buffer.width;
  buf->height = ctx->buffer.height;
  buf->depth_max = 65535;

  _sgl_init_renderbuffer(&buf->clear_color_buf, GL_UNSIGNED_INT, GL_TRUE);
  _sgl_init_renderbuffer(&buf->clear_depth_buf, GL_FLOAT, GL_TRUE);

  _sgl_init_renderbuffer(&buf->t_color_buf, GL_UNSIGNED_INT, GL_TRUE);
  _sgl_init_renderbuffer(&buf->t_depth_buf, GL_FLOAT, GL_TRUE);
  _sgl_init_renderbuffer(&buf->t_normal_buf, GL_UNSIGNED_INT, GL_TRUE);

  _sgl_init_renderbuffer(&buf->color_buf, GL_UNSIGNED_INT, GL_FALSE);
  _sgl_init_renderbuffer(&buf->depth_buf, GL_FLOAT, GL_TRUE);
  _sgl_init_renderbuffer(&buf->normal_buf, GL_UNSIGNED_INT, GL_TRUE);

  buf->r_color_buf = &buf->color_buf;
  buf->r_depth_buf = &buf->depth_buf;

  ctx->drawbuffer = buf;
}

void _sgl_free_framebuffer_data(struct sgl_context* ctx)
{
  _sgl_free_renderbuffer(&ctx->drawbuffer->clear_color_buf);
  _sgl_free_renderbuffer(&ctx->drawbuffer->clear_depth_buf);

  _sgl_free_renderbuffer(&ctx->drawbuffer->t_color_buf);
  _sgl_free_renderbuffer(&ctx->drawbuffer->t_depth_buf);
  _sgl_free_renderbuffer(&ctx->drawbuffer->t_normal_buf);

  _sgl_free_renderbuffer(&ctx->drawbuffer->color_buf);
  _sgl_free_renderbuffer(&ctx->drawbuffer->depth_buf);
  _sgl_free_renderbuffer(&ctx->drawbuffer->normal_buf);
}

void _sgl_clear_framebuffer(GLenum mask)
{
  GET_CURRENT_CONTEXT(ctx);
  GLint size = ctx->buffer.width * ctx->buffer.height;

  if (mask & GL_COLOR_BUFFER_BIT)
    memcpy(ctx->drawbuffer->color_buf.data,
           ctx->drawbuffer->clear_color_buf.data,
           size * sizeof(GLuint));

  if (mask & GL_DEPTH_BUFFER_BIT)
    memcpy(ctx->drawbuffer->depth_buf.data,
           ctx->drawbuffer->clear_depth_buf.data,
           size * sizeof(GLfloat));
}

void _sgl_framebuffer_depth(GLboolean status)
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->depth.test = status;

  if (status) {
    ctx->drawbuffer->r_color_buf = &ctx->drawbuffer->t_color_buf;
    ctx->drawbuffer->r_depth_buf = &ctx->drawbuffer->t_depth_buf;
  } else {
    ctx->drawbuffer->r_color_buf = &ctx->drawbuffer->color_buf;
    ctx->drawbuffer->r_depth_buf = &ctx->drawbuffer->depth_buf;
  }
}
