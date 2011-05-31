#include "sglContext.h"

struct SGLContext _g_sgl_context;

void sgl_context_init(struct SGLContext* sc)
{
  sc->matrix_mode = GL_MODELVIEW;
  _math_matrix_init(sc->matrix);
  sc->projection_matrix_stack = NULL;
  sc->modelview_matrix_stack = NULL;
  sc->flags = 0;
  sc->clientstate_flags = 0;
}

void sgl_context_free(struct SGLContext* sc)
{
  _math_matrix_free(sc->matrix);

  SList* current = s_list_first(sc->projection_matrix_stack);
  while (current) {
    _math_matrix_free((SGLmatrix*)current->data);
    current = s_list_next(current);
  }

  current = s_list_first(sc->modelview_matrix_stack);
  while (current) {
    _math_matrix_free((SGLmatrix*)current->data);
    current = s_list_next(current);
  }
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
