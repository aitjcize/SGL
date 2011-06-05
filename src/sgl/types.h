#ifndef __SGL_TYPES_H__
#define __SGL_TYPES_H__

#include "sglheader.h"
#include "config.h"
#include "math/m_matrix.h"
#include "math/m_vector.h"

#define PRIM_OUTSIDE_BEGIN_END   (GL_POLYGON+1)
#define PRIM_INSIDE_UNKNOW_PRIM  (GL_POLYGON+2)
#define PRIM_UNKNOWN             (GL_POLYGON+3)

struct sgl_buffer_attrib
{
  GLuint width, height;
  GLuint type;
};

struct sgl_renderbuffer
{
  GLuint width, height;     /* Geometry of buffer */
  GLuint type;              /* storage type */
  GLvoid* data;             /* Buffer data */
};

struct sgl_framebuffer
{
  GLboolean initialized;

  GLuint width, height;
  GLfloat depth_max;

  /* Buffers */
  struct sgl_renderbuffer color_buffer;
  struct sgl_renderbuffer depth_buffer;
};

struct sgl_matrix_stack
{
  SGLmatrix* top;           /* points into stack */
  SGLmatrix* stack;         /* array of [max_depth] of SGLMatrix */
  GLuint depth;             /* 0 <= depth < max_depth */
  GLuint max_depth;         /* size of stack[] array */
};

struct sgl_depthbuffer_attrib
{
  GLenum func;	            /* Function for depth buffer compare */
  GLclampd clear;           /* Value to clear depth buffer to */
  GLboolean test;           /* Depth buffering enabled flag */
  GLboolean mask;           /* Depth buffer writable? */
};

struct sgl_viewport_attrib
{
  GLint x, y;               /* possition */
  GLsizei width, height;    /* dimenssion */
  GLfloat near, far;        /* near, far value */
  SGLmatrix window_map;     /* Mapping transformation as a matrix */
};

struct sgl_render_state
{
#define FLUSH_STORED_VERTICES 0x1
  GLint current_exec_primitive;
  GLint needflush;
};

struct sgl_context
{
  /* Context */
  GLclampf clear_color[4];

  /* Framebuffers */
  struct sgl_framebuffer* drawbuffer;

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

  /* Attributes */
  struct sgl_buffer_attrib buffer;
  struct sgl_viewport_attrib viewport;
  struct sgl_depthbuffer_attrib depth;

  /* States */
  struct sgl_render_state render_state;

  /* Flags */
  GLenum flags;
  GLenum clientstate_flags;
};

#endif /* __SGL_TYPES_H__ */
