#include "depth.h"

#include "context.h"
#include "logging.h"
#include "macros.h"

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

  depth = CLAMP( depth, 0.0, 1.0 );

  if (ctx->depth.clear == depth)
    return;

  ctx->depth.clear = depth;
}

void glClearDepthf(GLclampf depth)
{
  glClearDepth(depth);
}

void glDepthFunc(GLenum func)
{
  GET_CURRENT_CONTEXT(ctx);
  ASSERT_OUTSIDE_BEGIN_END(ctx);

  switch (func) {
  case GL_LESS:    /* (default) pass if incoming z < stored z */
  case GL_GEQUAL:
  case GL_LEQUAL:
  case GL_GREATER:
  case GL_NOTEQUAL:
  case GL_EQUAL:
  case GL_ALWAYS:
  case GL_NEVER:
    break;
  default:
    _sgl_error(ctx, GL_INVALID_ENUM, "glDepth.Func");
    return;
  }

  if (ctx->depth.func == func)
    return;

  ctx->depth.func = func;
}

void glDepthMask(GLboolean flag)
{
  GET_CURRENT_CONTEXT(ctx);
  ASSERT_OUTSIDE_BEGIN_END(ctx);
  ctx->depth.mask = flag;
}
