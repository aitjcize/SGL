#include "sglContext.h"
#include "sglMatrixOp.h"

extern SGLContext* s_sgl;

void glPushMatrix(void)
{
  s_list_append(s_sgl->matrix_stack, s_sgl->matrix);
}

void glPopMatrix(void)
{
  SList* last = s_list_remove_link(s_sgl->matrix_stack,
                                   s_list_last(s_sgl->matrix_stack));
  s_sgl->matrix = (SGLmatrix*)last->data;
}

void glLoadIdentity(void)
{
  _sgl_matrix_set_identity(s_sgl->matrix);
}

void glLoadMatrixd(const GLdouble* m)
{
  _sgl_matrix_loadf(s_sgl->matrix, (const GLfloat*)m);
}

void glLoadMatrixf(const GLfloat* m)
{
  _sgl_matrix_loadf(s_sgl->matrix, m);
}

void glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
  _sgl_matrix_translate(s_sgl->matrix, x, y, z);
}

void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
  _sgl_matrix_rotate(s_sgl->matrix, angle, x, y, z);
}

void glScalef(GLfloat x, GLfloat y, GLfloat z)
{
  _sgl_matrix_scale(s_sgl->matrix, x, y, z);
}
