#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>

#include "context.h"
#include "logging.h"
#include "math/m_matrix.h"

static void init_matrix_stack(struct sgl_matrix_stack* stack, GLuint max_depth)
{
  GLuint i = 0;
  stack->depth = 0;
  stack->max_depth = max_depth;

  stack->stack = (SGLmatrix*) malloc(max_depth * sizeof(SGLmatrix));
  for (i = 0; i < max_depth; ++i) {
    _math_matrix_init(&stack->stack[i]);
    _math_matrix_alloc_inv(&stack->stack[i]);
  }
  stack->top = stack->stack;
}

static void free_matrix_stack(struct sgl_matrix_stack* stack)
{
  GLuint i = 0;

  for (i = 0; i < stack->max_depth; ++i)
    _math_matrix_free(&stack->stack[i]);
  free(stack->stack);
  stack->stack = stack->top = NULL;
}

void _sgl_init_matrix(struct sgl_context* ctx)
{
  init_matrix_stack(&ctx->modelview_matrix_stack, MAX_MODELVIEW_STACK_DEPTH);
  init_matrix_stack(&ctx->projection_matrix_stack, MAX_PROJECTION_STACK_DEPTH);
  ctx->current_stack = &ctx->modelview_matrix_stack;
  _math_matrix_init(&ctx->model_projection_matrix);
}

void _sgl_free_matrix_data(struct sgl_context* ctx)
{
  free_matrix_stack(&ctx->modelview_matrix_stack);
  free_matrix_stack(&ctx->projection_matrix_stack);
  _math_matrix_free(&ctx->model_projection_matrix);
}

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
  struct sgl_matrix_stack* stack = ctx->current_stack;

  if (stack->depth + 1 >= stack->max_depth) {
    _sgl_error(ctx, GL_STACK_OVERFLOW, "glPushMatrix(): stack overflow");
    return;
  }
  _math_matrix_copy(&stack->stack[stack->depth + 1],
                    &stack->stack[stack->depth]);
  stack->depth++;
  stack->top = &(stack->stack[stack->depth]);
}

void glPopMatrix(void)
{
  GET_CURRENT_CONTEXT(ctx);
  struct sgl_matrix_stack* stack = ctx->current_stack;

  if (stack->depth  == 0) {
    _sgl_error(ctx, GL_STACK_UNDERFLOW, "glPushMatrix(): stack underflow");
    return;
  }
  stack->depth--;
  stack->top = &(stack->stack[stack->depth]);
}

void glLoadIdentity(void)
{
  GET_CURRENT_CONTEXT(ctx);
  _math_matrix_set_identity(ctx->current_stack->top);
}

void glFrustum(GLdouble left, GLdouble right,
               GLdouble bottom, GLdouble top,
               GLdouble near_val, GLdouble far_val)
{
  GET_CURRENT_CONTEXT(ctx);
  _math_matrix_frustum(ctx->current_stack->top, left, right, bottom, top,
                       near_val, far_val);
}

void glOrtho(GLdouble left, GLdouble right,
             GLdouble bottom, GLdouble top,
             GLdouble near_val, GLdouble far_val)
{
  GET_CURRENT_CONTEXT(ctx);
  _math_matrix_ortho(ctx->current_stack->top, left, right, bottom, top,
                     near_val, far_val);
}

void glMultMatrixd(const GLdouble* m)
{
  glMultMatrixf((GLfloat*) m);
}

void glMultMatrixf(const GLfloat* m)
{
  GET_CURRENT_CONTEXT(ctx);
  _math_matrix_mul_floats(ctx->current_stack->top, m);
}

void glLoadMatrixd(const GLdouble* m)
{
  glLoadMatrixf((GLfloat*) m);
}

void glLoadMatrixf(const GLfloat* m)
{
  GET_CURRENT_CONTEXT(ctx);
  _math_matrix_loadf(ctx->current_stack->top, (const GLfloat*)m);
}

void glTranslated(GLdouble x, GLdouble y, GLdouble z)
{
  GET_CURRENT_CONTEXT(ctx);
  _math_matrix_translate(ctx->current_stack->top, x, y, z);
}

void glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
  GET_CURRENT_CONTEXT(ctx);
  _math_matrix_translate(ctx->current_stack->top, x, y, z);
}

void glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
  GET_CURRENT_CONTEXT(ctx);
  _math_matrix_rotate(ctx->current_stack->top, angle, x, y, z);
}

void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
  GET_CURRENT_CONTEXT(ctx);
  _math_matrix_rotate(ctx->current_stack->top, angle, x, y, z);
}

void glScalef(GLfloat x, GLfloat y, GLfloat z)
{
  GET_CURRENT_CONTEXT(ctx);
  _math_matrix_scale(ctx->current_stack->top, x, y, z);
}
