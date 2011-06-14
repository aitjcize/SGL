#include "pipeline.h"

#include <string.h>

#include "macros.h"
#include "rasterize.h"
#include "types.h"

void _sgl_init_pipeline(struct sgl_context* ctx)
{
  //ctx->pipeline.primitive_assembly = _sgl_primitive_assembly;
  ctx->pipeline.rasterize = _sgl_pipeline_rasterize;
}

void _sgl_pipeline_iteration(void)
{
  GET_CURRENT_CONTEXT(ctx);
  if (ctx->pipeline.primitive_assembly)
    ctx->pipeline.primitive_assembly();
  if (ctx->pipeline.vertex_shader)
    ctx->pipeline.vertex_shader();
  if (ctx->pipeline.rasterize)
    ctx->pipeline.rasterize();
  if (ctx->pipeline.fragment_shader)
    ctx->pipeline.fragment_shader();
}

void sglPipelineIter(char* buffer)
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->drawbuffer->color_buf.data = buffer;
}

void _sgl_pipeline_depth_test(void)
{
  GET_CURRENT_CONTEXT(ctx);
  GLint x = 0, y = 0;
  GLfloat dz = 0.0, nz = 0.0;
  struct sgl_framebuffer* buf = ctx->drawbuffer;
  GLint size = ctx->buffer.width * ctx->buffer.height;

  for (y = 0; y < buf->height; ++y)
    for (x = 0; x < buf->width; ++x) {
      dz = NORMALIZE_Z(ctx, BUF_GET_D(&buf->depth_buf, x, y));
      nz = NORMALIZE_Z(ctx, BUF_GET_D(&buf->t_depth_buf, x, y));

      if (nz < dz) {
        BUF_SET_C(&buf->color_buf, x, y, BUF_GET_C(&buf->t_color_buf, x, y));
        BUF_SET_D(&buf->depth_buf, x, y, BUF_GET_D(&buf->t_depth_buf, x, y));
      }
    }

  /* Clear temporal buffer */
  memcpy(ctx->drawbuffer->t_color_buf.data,
         ctx->drawbuffer->clear_color_buf.data,
         size * sizeof(GLuint));

  memcpy(ctx->drawbuffer->t_depth_buf.data,
         ctx->drawbuffer->clear_depth_buf.data,
         size * sizeof(GLfloat));
}
