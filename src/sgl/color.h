#ifndef __SGL_COLOR_BUF_H__
#define __SGL_COLOR_BUF_H__

#include "types.h"

extern void _sgl_init_color(struct sgl_context* ctx);

extern void glClearColor(GLclampf red, GLclampf green, GLclampf blue,
                         GLclampf alpha);


#endif /* __SGL_COLOR_BUF_H__ */
