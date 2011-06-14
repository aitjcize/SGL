#include "renderbuffer.h"

#include <stdlib.h>
#include <string.h>

void _sgl_init_renderbuffer(struct sgl_renderbuffer* buf, GLint type,
                            GLboolean allocate)
{
  GET_CURRENT_CONTEXT(ctx);
  GLuint size = 0;
  buf->width = ctx->buffer.width;
  buf->height = ctx->buffer.height;
  buf->type = type;
  buf->allocated = allocate;

  if (allocate) {
    size = ctx->buffer.width * ctx->buffer.height * _sgl_sizeof_type(type);
    buf->data = malloc(size);
    memset(buf->data, 0, size);
  }
}

void _sgl_free_renderbuffer(struct sgl_renderbuffer* buf)
{
  if (buf->allocated)
    free(buf->data);
}
