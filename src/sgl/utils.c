#include "utils.h"

GLint _sgl_sizeof_type(GLenum type)
{
  switch (type) {
  case GL_BITMAP:
    return 0;
  case GL_UNSIGNED_BYTE:
    return sizeof(GLubyte);
  case GL_BYTE:
    return sizeof(GLbyte);
  case GL_UNSIGNED_SHORT:
    return sizeof(GLushort);
  case GL_SHORT:
    return sizeof(GLshort);
  case GL_UNSIGNED_INT:
    return sizeof(GLuint);
  case GL_INT:
    return sizeof(GLint);
  case GL_FLOAT:
    return sizeof(GLfloat);
  case GL_DOUBLE:
    return sizeof(GLdouble);
  case GL_HALF_FLOAT_ARB:
    return sizeof(GLhalfARB);
  default:
    return -1;
  }
}
