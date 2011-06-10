#include "depth.h"

void _sgl_init_depth(struct sgl_context* ctx)
{
  ctx->depth.test = GL_FALSE;
  ctx->depth.clear = 1.0;
  ctx->depth.func = GL_LESS;
  ctx->depth.mask = GL_TRUE;
}

void glClearDepth(GLclampd depth)
{
  GET_CURRENT_CONTEXT(ctx);
  ASSERT_OUTSIDE_BEGIN_END(ctx);
  ctx->depth.clear = CLAMP(depth, 0, 1.0);
}

void glDepthFunc(GLenum func)
{
  GET_CURRENT_CONTEXT(ctx);
  ASSERT_OUTSIDE_BEGIN_END(ctx);
  switch (func) {
  case GL_NEVER:
  case GL_LESS:
  case GL_EQUAL:
  case GL_LEQUAL:
  case GL_GREATER:
  case GL_NOTEQUAL:
  case GL_GEQUAL:
  case GL_ALWAYS:
    break;
  default:
    _sgl_error(ctx, GL_INVALID_ENUM, "glDepthFunc(): Invalid mode\n");
    return;
  }
  ctx->depth.clear = depth;
}

void glDepthMask(GLboolean flag)
{
  GET_CURRENT_CONTEXT(ctx);
  ASSERT_OUTSIDE_BEGIN_END(ctx);
  ctx->depth.mask = flag;
}
