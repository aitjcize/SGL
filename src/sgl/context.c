#include "context.h"

struct sgl_context _g_sgl_context;

void sgl_context_init(struct sgl_context* ctx)
{
  ctx->matrix_mode = GL_MODELVIEW;
  _sgl_init_matrix_data(ctx);

  ctx->flags = 0;
  ctx->clientstate_flags = 0;
}

void sgl_context_free(struct sgl_context* ctx)
{
  _sgl_free_matrix_data(ctx);
}

void glClear(GLbitfield mask)
{
  GET_CURRENT_CONTEXT(ctx);
  _sgl_vector4f_free(&ctx->vector_point);
  _sgl_vector4f_free(&ctx->vector_normal);
  _sgl_vector4f_free(&ctx->vector_color);
  _sgl_vector4f_alloc(&ctx->vector_color, 0, SGL_INIT_VERTEX_LEN);
  _sgl_vector4f_alloc(&ctx->vector_color, 0, SGL_INIT_VERTEX_LEN);
  _sgl_vector4f_alloc(&ctx->vector_color, 0, SGL_INIT_VERTEX_LEN);
}

void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->clear_color[0] = red;
  ctx->clear_color[1] = green;
  ctx->clear_color[2] = blue;
  ctx->clear_color[3] = alpha;
}

void glEnableClientState(GLenum cap)
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->clientstate_flags |= cap;
}

void glDisableClientState(GLenum cap)
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->clientstate_flags &= ~cap;
}
