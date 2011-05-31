#ifndef __SGLUT_SGLUTAPI_H__
#define __SGLUT_SGLUTAPI_H__

extern struct SGLUTContext _g_sglut_context;

#define GET_CURRENT_CONTEXT(C) \
  struct SGLUTContext* C = (struct SGLUTContext*) &_g_sglut_context

#endif /* __SGLUT_SGLUTAPI_H__ */
