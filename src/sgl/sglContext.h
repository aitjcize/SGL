#ifndef __SGL_SGLCONTEXT_H__
#define __SGL_SGLCONTEXT_H__

#include "sglHeaders.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "utils/list.h"

#define SGL_INIT_VERTEX_LEN 512

struct SGLContext {
  /* Context */
  GLclampf clear_color[4];

  /* Matrix */
  GLint matrix_mode;
  SGLmatrix* matrix;
  SList** current_stack;
  SList* projection_matrix_stack;
  SList* modelview_matrix_stack;

  /* Geometry Buffers */
  SGLvector4f vector_point;
  SGLvector4f vector_normal;
  SGLvector4f vector_color;

  /* Geometry Pointers */
  SGLvector4f vertex_pointer;
  SGLvector4f normal_pointer;
  SGLvector4f color_pointer;

  /* ClientState Flags */
  GLuint flags;
  GLuint clientstate_flags;
};

void sgl_context_init(struct SGLContext* sc);

void sgl_context_free(struct SGLContext* sc);

extern void glClear(GLbitfield mask);

extern void glClearColor(GLclampf red, GLclampf green, GLclampf blue,
                         GLclampf alpha);

extern void glBegin(GLenum mode);

extern void glEnd(void);

extern void glEnableClientState(GLenum cap);

extern void glDisableClientState(GLenum cap);

#endif /* __SGL_SGLCONTEXT_H__ */
