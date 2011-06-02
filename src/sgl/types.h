#ifndef __SGL_TYPES_H__
#define __SGL_TYPES_H__

#include "sgl_headers.h"
#include "config.h"
#include "math/m_matrix.h"
#include "math/m_vector.h"

#define SGL_INIT_VERTEX_LEN 512

struct sgl_framebuffer {
  GLboolean initialized;

  GLuint width;
  GLuint height;
};

struct sgl_matrix_stack {
  SGLmatrix* top;           /* points into stack */
  SGLmatrix* stack;         /* array of [max_depth] of SGLMatrix */
  GLuint depth;             /* 0 <= depth < max_depth */
  GLuint max_depth;         /* size of stack[] array */
};

struct sgl_context {
  /* Context */
  GLclampf clear_color[4];

  /* Framebuffers */
  struct sgl_framebuffer* draw_buffer;

  /* Matrix */
  GLint matrix_mode;
  SGLmatrix model_projection_matrix;
  struct sgl_matrix_stack* current_stack;
  struct sgl_matrix_stack projection_matrix_stack;
  struct sgl_matrix_stack modelview_matrix_stack;

  /* Geometry Buffers */
  SGLvector4f vector_point;
  SGLvector4f vector_normal;
  SGLvector4f vector_color;

  /* Geometry Pointers */
  SGLvector4f vertex_pointer;
  SGLvector4f normal_pointer;
  SGLvector4f color_pointer;

  /* ClientState Flags */
  GLenum flags;
  GLenum clientstate_flags;
};

#endif /* __SGL_TYPES_H__ */
