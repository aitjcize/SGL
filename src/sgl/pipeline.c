#include "pipeline.h"

#include <string.h>

#include "rasterize.h"
#include "types.h"

void _sgl_init_pipeline(struct sgl_context* ctx)
{
  //ctx->pipeline.primitive_assembly = _sgl_primitive_assembly;
  ctx->pipeline.rasterize = _sgl_pipeline_rasterize;
  ctx->pipeline.depth_test = _sgl_pipeline_depth_test;
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
  if (ctx->pipeline.depth_test)
    ctx->pipeline.depth_test();

  memcpy(ctx->drawbuffer->final_buffer.data,
         ctx->drawbuffer->color_buf.data,
         ctx->buffer.width * ctx->buffer.height * 4);
}

void sglPipelineIter(char* buffer)
{
  GET_CURRENT_CONTEXT(ctx);
  ctx->drawbuffer->final_buffer.data = buffer;
}

void _sgl_pipeline_depth_test(void)
{

}
