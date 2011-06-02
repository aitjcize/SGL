#include "sglMatrix.h"

#include <stdio.h>

#include "sglContext.h"

void glMatrixMode(GLenum mode)
{
  GET_CURRENT_CONTEXT(ctx);
  switch (mode) {
  case GL_MODELVIEW:
    ctx->current_stack = &ctx->modelview_matrix_stack;
    break;
  case GL_PROJECTION:
    ctx->current_stack = &ctx->projection_matrix_stack;
    break;
  default:
    fprintf(stderr, "glMatrixMode(): mode not implemented!\n");
  }
}

void glPushMatrix(void)
{
  GET_CURRENT_CONTEXT(ctx);
  s_list_append(*(ctx->current_stack), ctx->matrix);
}

void glPopMatrix(void)
{
  GET_CURRENT_CONTEXT(ctx);
  SList* last = s_list_last(*(ctx->current_stack));
  s_list_remove_link(*(ctx->current_stack), last);
  _math_matrix_free((SGLmatrix*)last->data);

  last = s_list_last(*(ctx->current_stack));
  ctx->matrix = (SGLmatrix*)last->data;
}

void glLoadIdentity(void)
{
  GET_CURRENT_CONTEXT(ctx);
  _math_matrix_set_identity(ctx->matrix);
}

void glLoadMatrixd(const GLdouble* m)
{
  GET_CURRENT_CONTEXT(ctx);
  _math_matrix_loadf(ctx->matrix, (const GLfloat*)m);
}

void glLoadMatrixf(const GLfloat* m)
{
  GET_CURRENT_CONTEXT(ctx);
  _math_matrix_loadf(ctx->matrix, m);
}

void glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
  GET_CURRENT_CONTEXT(ctx);
  _math_matrix_translate(ctx->matrix, x, y, z);
}

void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
  GET_CURRENT_CONTEXT(ctx);
  _math_matrix_rotate(ctx->matrix, angle, x, y, z);
}

void glScalef(GLfloat x, GLfloat y, GLfloat z)
{
  GET_CURRENT_CONTEXT(ctx);
  _math_matrix_scale(ctx->matrix, x, y, z);
}
