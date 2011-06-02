#ifndef __SGLUT_API_H__
#define __SGLUT_API_H__

extern struct sglut_context _g_sglut_context;

#define GET_CURRENT_CONTEXT(C) \
  struct sglut_context* C = (struct sglut_context*) &_g_sglut_context

#endif /* __SGLUT_API_H__ */
