#include "sglContext.h"

SGLContext g_sgl_context;

void sgl_context_init(SGLContext* sc)
{
  sc->matrix_mode = GL_MODELVIEW;
  _sgl_matrix_init(sc->matrix);
  sc->matrix_stack = NULL;
}

void sgl_context_free(SGLContext* sc)
{
  _sgl_matrix_free(sc->matrix);

  SList* current = s_list_first(sc->matrix_stack);

  while (current) {
    _sgl_matrix_free((SGLmatrix*)current->data);
    current = s_list_next(current);
  }
}

void glClear(GLbitfield mask)
{
  _sgl_vector4f_free(&g_sgl_context.vector_point);
  _sgl_vector4f_free(&g_sgl_context.vector_normal);
  _sgl_vector4f_free(&g_sgl_context.vector_color);
  _sgl_vector4f_alloc(&g_sgl_context.vector_color, 0, SGL_INIT_VERTEX_LEN);
  _sgl_vector4f_alloc(&g_sgl_context.vector_color, 0, SGL_INIT_VERTEX_LEN);
  _sgl_vector4f_alloc(&g_sgl_context.vector_color, 0, SGL_INIT_VERTEX_LEN);
}

void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
  g_sgl_context.clear_color[0] = red;
  g_sgl_context.clear_color[1] = green;
  g_sgl_context.clear_color[2] = blue;
  g_sgl_context.clear_color[3] = alpha;
}

void glEnableClientState(GLenum cap)
{
  g_sgl_context.clientstate_flags |= cap;
}

void glDisableClientState(GLenum cap)
{
  g_sgl_context.clientstate_flags &= ~cap;
}
