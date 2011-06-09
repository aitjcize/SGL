#include "renderbuffer.h"

#include <stdlib.h>
#include <string.h>

void _sgl_init_renderbuffer(struct sgl_renderbuffer* buf, int type)
{
  GET_CURRENT_CONTEXT(ctx);
  GLuint size = ctx->buffer.width * ctx->buffer.height * sizeof(type);
  buf->width = ctx->buffer.width;
  buf->height = ctx->buffer.height;
  buf->type = type;
  buf->data = malloc(size);
  memset(buf->data, 0, size);
}

void _sgl_free_renderbuffer(struct sgl_renderbuffer* buf)
{
  free(buf->data);
}
