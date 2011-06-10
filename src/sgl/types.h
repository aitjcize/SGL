#ifndef __SGL_TYPES_H__
#define __SGL_TYPES_H__

#include "sglheader.h"
#include "config.h"
#include "pipeline.h"
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
  GLuint width, height;     /* Dimemsion */
  GLuint type;              /* Storage type */
  GLchar* data;            /* Buffer data */
};

struct sgl_framebuffer
{
  GLboolean initialized;

  GLuint width, height;
  GLfloat depth_max;

  /* Buffers */
  struct sgl_renderbuffer* r_color_buf;
  struct sgl_renderbuffer* r_depth_buf;

  struct sgl_renderbuffer final_buffer;
  struct sgl_renderbuffer clear_color_buf;
  struct sgl_renderbuffer clear_depth_buf;

  struct sgl_renderbuffer t_color_buf;
  struct sgl_renderbuffer t_normal_buf;
  struct sgl_renderbuffer t_depth_buf;

  struct sgl_renderbuffer color_buf;
  struct sgl_renderbuffer normal_buf;
  struct sgl_renderbuffer depth_buf;
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

struct sgl_colorbuffer_attrib
{
  GLclampd clear;           /* Value to clear depth buffer to */
};

struct sgl_viewport_attrib
{
  GLint x, y;               /* possition */
  GLsizei width, height;    /* dimenssion */
  GLfloat near, far;        /* near, far value */
  SGLmatrix window_map;     /* Mapping transformation as a matrix */
};

struct sgl_polygon_attrib
{
  GLint front;
  GLint back;
};

struct sgl_primitive_attrib
{
  GLfloat color[4];
  GLfloat normal[4];
};

struct sgl_vertex_array_attrib {
  GLenum mode;
  GLint count;
  GLenum type;
  GLvoid* indices_ptr;
};

struct sgl_render_state
{
#define FLUSH_STORED_VERTICES 0x1
  GLint current_exec_primitive;   /* Current executed primitive */
  GLint type;                     /* 0 or GL_VERTEX_ARRAY */
  GLint needflush;
};

struct sgl_pipeline
{
  void (*primitive_assembly)(void);
  void (*vertex_shader)(void);
  void (*rasterize)(void);
  void (*fragment_shader)(void);
  void (*depth_test)(void);
};

struct sgl_context
{
  /* Framebuffers */
  struct sgl_framebuffer* drawbuffer;

  /* Pipeline */
  struct sgl_pipeline pipeline;

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
  SGLvector4f flood_fill;

  /* Geometry Pointers */
  SGLvector4f vertex_pointer;
  SGLvector4f normal_pointer;
  SGLvector4f color_pointer;

  /* Attributes */
  struct sgl_buffer_attrib buffer;
  struct sgl_viewport_attrib viewport;
  struct sgl_colorbuffer_attrib color;
  struct sgl_depthbuffer_attrib depth;
  struct sgl_polygon_attrib polygon;
  struct sgl_primitive_attrib primitive;
  struct sgl_vertex_array_attrib varray;

  /* States */
  struct sgl_render_state render_state;

  /* Flags */
  GLenum flags;
  GLenum clientstate_flags;
};

#endif /* __SGL_TYPES_H__ */
