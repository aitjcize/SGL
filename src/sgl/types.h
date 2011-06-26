/**
 * @file   types.h
 *
 * Copyright (C) 2011 - SGL Authors <aitjcize@gmail.com>
 * All Rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
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

struct sgl_renderbuffer
{
  GLuint width, height;     /* Dimemsion */
  GLuint type;              /* Storage type */
  GLchar* data;             /* Buffer data */
  GLboolean allocated;      /* Buffer allocated */
};

struct sgl_framebuffer
{
  GLboolean initialized;

  GLuint width, height;
  GLfloat depth_max;

  /* Buffers */
  struct sgl_renderbuffer* r_color_buf;
  struct sgl_renderbuffer* r_depth_buf;

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
  GLenum func;              /* Function for depth buffer compare */
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
  const GLvoid* indices_ptr;
};

struct sgl_render_state
{
#define FLUSH_STORED_VERTICES 0x1
  GLint current_exec_primitive;   /* Current executed primitive */
  GLint type;                     /* 0 or GL_VERTEX_ARRAY */
  GLint needflush;
  GLint* edge_tab;
};

struct sgl_light
{
   struct gl_light *next;       /* double linked list with sentinel */
   struct gl_light *prev;

   GLfloat ambient[4];          /* ambient color */
   GLfloat diffuse[4];          /* diffuse color */
   GLfloat specular[4];         /* specular color */
   GLfloat eye_position[4];     /* position in eye coordinates */
   GLfloat spot_direction[4];   /* spotlight direction in eye coordinates */
   
   GLfloat spot_exponent;
   GLfloat spot_cutoff;         /* in degrees */
   GLfloat _cos_cutoff_neg;     /* cos(SpotCutoff) */
   GLfloat _cos_cutoff;         /* MAX(0, cos(SpotCutoff)) */
   
   GLfloat constant_attenuation;
   GLfloat linear_attenuation;
   GLfloat quadratic_attenuation;
   GLboolean enabled;           /* On/off flag */

   GLfloat _position[4];        /* position in eye/obj coordinates */
   GLfloat _vp_inf_norm[3];     /* Norm direction to infinite light */
   GLfloat _h_inf_norm[3];      /* Norm( _VP_inf_norm + <0,0,1> ) */
   GLfloat _normspot_direction[4]; /* normalized spotlight direction */
   GLfloat _vp_inf_spot_attenuation;

   GLfloat _mat_ambient[2][3];  /* material ambient * light ambient */
   GLfloat _mat_diffuse[2][3];  /* material diffuse * light diffuse */
   GLfloat _mat_specular[2][3]; /* material spec * light specular */
};

struct sgl_lightmodel
{
   GLfloat Ambient[4];     /* ambient color */
   GLboolean LocalViewer;  /* Local (or infinite) view point */
   GLboolean TwoSide;      /* Two (or one) sided lighting */
   GLenum ColorControl;    /* either GL_SINGLE_COLOR or
                            * GL_SEPARATE_SPECULAR_COLOR */
};

struct sgl_material
{
   GLfloat Attrib[MAT_ATTRIB_MAX][4];
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
