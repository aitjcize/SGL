#include "renderbuffer.h"

#include <stdlib.h>

void _sgl_init_renderbuffer(struct sgl_renderbuffer* buf, int type)
{
  GET_CURRENT_CONTEXT(ctx);
  buf->width = ctx->buffer.width;
  buf->height = ctx->buffer.height;
  buf->type = type;
  buf->data = malloc(buf->width * buf->height * sizeof(type));
}

void _sgl_free_renderbuffer(struct sgl_renderbuffer* buf)
{
  free(buf->data);
}
