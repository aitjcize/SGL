#include "sglCOntext.h"
#include "sglMatrixOp.h"

extern SGLContext* g_sgl_context;

void glPushMatrix(void)
{
  s_list_append(g_sgl_context->matrix_stack, g_sgl_context->matrix);
}

void glPopMatrix(void)
{
  SList* last = s_list_remove_link(g_sgl_context->matrix_stack,
                                   s_list_last(g_sgl_context));
  g_sgl_context->matrix = (SGLmatrix*)last->data;
}

void glLoadIdentity(void)
{
  _sgl_matrix_set_identity(g_sgl_context->matrix);
}

void glLoadMatrixd(const GLdouble* m)
{
  _sgl_matrix_loadf(g_sgl_context->matrix, (const GLfloat*)m);
}

void glLoadMatrixf(const GLfloat* m)
{
  _sgl_matrix_loadf(g_sgl_context->matrix, m);
}

void glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
  _sgl_matrix_translate(g_sgl_context->matrix, x, y, z);
}

void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
  _sgl_matrix_rotate(g_sgl_context->matrix, angle, x, y, z);
}

void glScalef(GLfloat x, GLfloat y, GLfloat z)
{
  _sgl_matrix_scale(g_sgl_context->matrix, x, y, z);
}
