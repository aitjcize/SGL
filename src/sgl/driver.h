#ifndef __SGL_DRIVER_H__
#define __SGL_DRIVER_H__

#define PRIM_OUTSIDE_BEGIN_END   (GL_POLYGON+1)
#define PRIM_INSIDE_UNKNOW_PRIM  (GL_POLYGON+2)
#define PRIM_UNKNOWN             (GL_POLYGON+3)

struct sgl_virtual_dirver_table
{
  GLint current_exec_primitive;
  GLint needflush;

  void (*primitive_assembly)(void);
  void (*vertex_shader)(void);
  void (*fragment_shader)(void);
  void (*zbuffer_test)(void);
};

#endif /* __SGL_DRIVER_H__ */
