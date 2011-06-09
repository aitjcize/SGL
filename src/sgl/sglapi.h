#ifndef __SGL_SGLAPI_H__
#define __SGL_SGLAPI_H__

extern struct sgl_context _g_sgl_context;

#define GET_CURRENT_CONTEXT(C) \
  struct sgl_context* C = (struct sgl_context*) &_g_sgl_context

extern void sglInit(int w, int h);

extern void sglPipelineIter(char* buffer);

#endif /* __SGL_SGLAPI_H__ */
