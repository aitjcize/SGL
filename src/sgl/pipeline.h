#ifndef __SGL_PIPLINE_H__
#define __SGL_PIPLINE_H__

struct sgl_pipeline
{
  void (*primitive_assembly)(void);
  void (*vertex_shader)(void);
  void (*rasterize)(void);
  void (*fragment_shader)(void);
  void (*zbuffer_test)(void);
}

#endif /* __SGL_PIPLINE_H__ */
