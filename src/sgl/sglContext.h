#ifndef __SGL_SGLCONTEXT_H__
#define __SGL_SGLCONTEXT_H__

#include "glHeaders.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "utils/list.h"

#define SGL_INIT_VERTEX_LEN 512

typedef struct _SGLContext SGLContext;
struct _SGLContext {
  /* Context */
  GLclampf clear_color[4];

  /* Matrix */
  GLint matrix_mode;
  SGLmatrix* matrix;
  SList* matrix_stack;

  /* Geometry Buffers */
  SGLvector4f vector_point;
  SGLvector4f vector_normal;
  SGLvector4f vector_color;

  /* Geometry Pointers */
  SGLvector4f vertex_pointer;
  SGLvector4f normal_pointer;
  SGLvector4f color_pointer;

  /* ClientState Flags */
  GLuint clientstate_flags;
};

void sgl_context_init(SGLContext* sc);

void sgl_context_free(SGLContext* sc);

extern void glClear(GLbitfield mask);

extern void glClearColor(GLclampf red, GLclampf green, GLclampf blue,
                         GLclampf alpha);

void glEnableClientState(GLenum cap);

void glDisableClientState(GLenum cap);

#endif /* __SGL_SGLCONTEXT_H__ */
