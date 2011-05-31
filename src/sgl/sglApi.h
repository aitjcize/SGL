#ifndef __SGL_SGLAPI_H__
#define __SGL_SGLAPI_H__

extern struct SGLContext _g_sgl_context;

#define GET_CURRENT_CONTEXT(C) \
  struct SGLContext* C = (struct SGLContext*) &_g_sgl_context

#endif /* __SGL_SGLAPI_H__ */
