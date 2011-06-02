#ifndef __SGL_CONTEXT_H__
#define __SGL_CONTEXT_H__

#include "types.h"

void _sgl_context_init(struct sgl_context* sc);

void _sgl_context_free(struct sgl_context* sc);

extern void glClear(GLbitfield mask);

extern void glClearColor(GLclampf red, GLclampf green, GLclampf blue,
                         GLclampf alpha);

extern void glBegin(GLenum mode);

extern void glEnd(void);

extern void glEnableClientState(GLenum cap);

extern void glDisableClientState(GLenum cap);

#endif /* __SGL_CONTEXT_H__ */
