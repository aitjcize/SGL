/**
 * @file   sgl/types.h
 *
 * Copyright (C) 2011 - SGL Authors <aitjcize@gmail.com>
 * All Rights reserved.
 *
 * This file is partially modified from the Mesa 3D project
 * Copyright (C) 1999-2008  Brian Paul   All Rights Reserved.
 * Copyright (C) 2009  VMware, Inc.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

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

/* Material Bit flags */
#define MAT_ATTRIB_FRONT_AMBIENT           0 
#define MAT_ATTRIB_BACK_AMBIENT            1
#define MAT_ATTRIB_FRONT_DIFFUSE           2 
#define MAT_ATTRIB_BACK_DIFFUSE            3
#define MAT_ATTRIB_FRONT_SPECULAR          4 
#define MAT_ATTRIB_BACK_SPECULAR           5
#define MAT_ATTRIB_FRONT_EMISSION          6
#define MAT_ATTRIB_BACK_EMISSION           7
#define MAT_ATTRIB_FRONT_SHININESS         8
#define MAT_ATTRIB_BACK_SHININESS          9
#define MAT_ATTRIB_FRONT_INDEXES           10
#define MAT_ATTRIB_BACK_INDEXES            11
#define MAT_ATTRIB_MAX                     12

struct sgl_buffer_attrib
{
  GLuint width, height;
  GLuint type;
};

/**
 * @struct sgl_renderbuffer
 */
struct sgl_renderbuffer
{
  GLuint width, height;     /**< Dimemsion */
  GLuint type;              /**< Storage type */
  GLchar* data;             /**< Buffer data */
  GLboolean allocated;      /**< Buffer allocated */
};

/**
 * @brief structure to store all colorbuffers
 * @struct sgl_framebuffer
 */
struct sgl_framebuffer
{
  GLboolean initialized;

  GLuint width, height;
  GLfloat depth_max;                        /**< Max depth buffer depth */

  /* Buffers */
  struct sgl_renderbuffer clear_color_buf;  /**< clear colorbuffer */
  struct sgl_renderbuffer clear_depth_buf;  /**< clear depthbuffer */

  struct sgl_renderbuffer color_buf;        /**< Colorbuffer */
  struct sgl_renderbuffer depth_buf;        /**< Depthbuffer */

  GLint* edge_tab;                          /**< Edge table for scanline fill */
};

struct sgl_matrix_stack
{
  SGLmatrix* top;           /**< points into stack */
  SGLmatrix* stack;         /**< array of [max_depth] of SGLMatrix */
  GLuint depth;             /**< 0 <= depth < max_depth */
  GLuint max_depth;         /**< size of stack[] array */
};

struct sgl_depthbuffer_attrib
{
  GLenum func;              /**< Function for depth buffer compare */
  GLclampd clear;           /**< Value to clear depth buffer to */
  GLboolean test;           /**< Depth buffering enabled flag */
  GLboolean mask;           /**< Depth buffer writable? */
};

struct sgl_colorbuffer_attrib
{
  GLclampd clear;           /**< Value to clear depth buffer to */
};

struct sgl_viewport_attrib
{
  GLint x, y;               /**< possition */
  GLsizei width, height;    /**< dimenssion */
  GLfloat near, far;        /**< near, far value */
  SGLmatrix window_map;     /**< Mapping transformation as a matrix */
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
  const GLvoid* indices_ptr;
};

struct sgl_render_state
{
#define FLUSH_STORED_VERTICES 0x1
  GLint current_exec_primitive;   /**< Current executed primitive */
  GLint type;                     /**< 0 or GL_VERTEX_ARRAY */
  GLint needflush;                /**< Where we need to flush the vertices */
  GLboolean gfill;                /**< Wether a geometry needs to be fill */
};

struct sgl_light
{
   struct gl_light *next;       /**< double linked list with sentinel */
   struct gl_light *prev;

   GLfloat ambient[4];          /**< ambient color */
   GLfloat diffuse[4];          /**< diffuse color */
   GLfloat specular[4];         /**< specular color */
   GLfloat eye_position[4];     /**< position in eye coordinates */
   GLfloat spot_direction[4];   /**< spotlight direction in eye coordinates */
   
   GLfloat spot_exponent;
   GLfloat spot_cutoff;         /**< in degrees */
   GLfloat _cos_cutoff_neg;     /**< cos(SpotCutoff) */
   GLfloat _cos_cutoff;         /**< MAX(0, cos(SpotCutoff)) */
   
   GLfloat constant_attenuation;
   GLfloat linear_attenuation;
   GLfloat quadratic_attenuation;
   GLboolean enabled;           /**< On/off flag */

   GLfloat _position[4];        /**< position in eye/obj coordinates */
   GLfloat _vp_inf_norm[3];     /**< Norm direction to infinite light */
   GLfloat _h_inf_norm[3];      /**< Norm( _VP_inf_norm + <0,0,1> ) */
   GLfloat _normspot_direction[4]; /**< normalized spotlight direction */
   GLfloat _vp_inf_spot_attenuation;

   GLfloat _mat_ambient[2][3];  /**< material ambient * light ambient */
   GLfloat _mat_diffuse[2][3];  /**< material diffuse * light diffuse */
   GLfloat _mat_specular[2][3]; /**< material spec * light specular */
};

struct sgl_lightmodel
{
   GLfloat Ambient[4];     /**< ambient color */
   GLboolean LocalViewer;  /**< Local (or infinite) view point */
   GLboolean TwoSide;      /**< Two (or one) sided lighting */
   GLenum ColorControl;    /**< either GL_SINGLE_COLOR or
                            * GL_SEPARATE_SPECULAR_COLOR */
};

struct sgl_material
{
   GLfloat Attrib[MAT_ATTRIB_MAX][4];
};

struct sgl_pipeline
{
  void (*vertex_shader)(void);
  void (*rasterize)(void);
  void (*fragment_shader)(void);
  void (*depth_test)(void);
};

/**
 * @brief SGL state machine main context
 * @sturct sgl_context
 */
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

/**
 * @struct color_t
 */
typedef union _color_t color_t;
union _color_t {
  GLuint val;
  struct {
    GLubyte a;
    GLubyte r;
    GLubyte g;
    GLubyte b;
  } c;
};

#endif /* __SGL_TYPES_H__ */
