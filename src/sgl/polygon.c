#include "polygon.h"

#include "logging.h"

void glPolygonMode(GLenum face, GLenum mode)
{
  GET_CURRENT_CONTEXT(ctx);

  switch (mode) {
  case GL_POINT:
  case GL_LINE:
  case GL_FILL:
    break;
  default:
    _sgl_error(ctx, GL_INVALID_ENUM, "glPolygonMode(): Invalid polygon mode\n");
    return;
  }

  switch (face) {
  case GL_FRONT:
    ctx->polygon.front = mode;
    break;
  case GL_BACK:
    ctx->polygon.back = mode;
    break;
  case GL_FRONT_AND_BACK:
    ctx->polygon.front = ctx->polygon.back = mode;
    break;
  default:
    _sgl_error(ctx, GL_INVALID_ENUM, "glPolygonMode(): Invalid face\n");
  }
}
