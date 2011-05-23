#include "sglContext.h"

extern SGLContext g_sgl_context;

void sgl_context_init(SGLContext* sc)
{
  sc->matrix_mode = GL_MODELVIEW;
  _sgl_matrix_init(sc->matrix);
  sc->matrix_stack = NULL;
}

void sgl_context_free(SGLContext* sc)
{
  _sgl_matrix_free(sc->matrix);

  SList* current = s_list_frist(sc->matrix_stack);

  while (current) {
    _sgl_matrix_free((SGLmatrix*)current->data);
    current = s_list_next(current);
  }
}
