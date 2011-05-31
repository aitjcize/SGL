/*
 * Vector Math Library
 *
 * Most of code in this file is taken from Mesa
 *
 */

#include "vector.h"
#include "utils.h"

void _sgl_vector4f_init(SGLvector4f *v, GLbitfield flags, GLfloat (*storage)[4])
{
  v->stride = 4 * sizeof(GLfloat);
  v->size = 2;
  v->data = storage;
  v->start = (GLfloat*) storage;
  v->count = 0;
}

void _sgl_vector4f_alloc(SGLvector4f *v, GLbitfield flags, GLuint count)
{
  v->stride = 4 * sizeof(GLfloat);
  v->size = 2;
  v->storage = malloc(count * 4 * sizeof(GLfloat));
  v->storage_count = count;
  v->start = (GLfloat *) v->storage;
  v->data = (GLfloat (*)[4]) v->storage;
  v->count = 0;
  v->flags = flags | VEC_MALLOC;
}

void _sgl_vector4f_alloc_double(SGLvector4f *v)
{
  void* new_storage = malloc(v->storage_count * 2 * 4 * sizeof(GLfloat));
  memcpy(new_storage, v->storage, v->storage_count * 4 * sizeof(GLfloat));
  free(v->storage);
  v->storage = new_storage;
  v->storage_count *= 2;
}

void _sgl_vector4f_free(SGLvector4f* v)
{
  if (v->flags & VEC_MALLOC) {
    free(v->storage);
    v->data = NULL;
    v->start = NULL;
    v->storage = NULL;
    v->flags &= ~VEC_MALLOC;
  }
}

GLboolean _sgl_vector4f_push_back(SGLvector4f* v, GLfloat* data, GLuint size)
{
  if (v->flags & VEC_MALLOC) {
    if (v->start == v->storage + v->storage_count)
      return GL_FALSE;
  } else {
    if (v->start == (void*)v->data + v->count)
      return GL_FALSE;
  }

  memcpy(v->start, data, size * sizeof(GLfloat));
  v->start += 1;

  return GL_TRUE;
}

GLboolean _sgl_vector4f_pop_back(SGLvector4f* v, GLfloat* data, GLuint size)
{
  if (v->start == (void*)v->data)
    return GL_FALSE;

  v->start -= 1;
  memcpy(data, v->start, size * sizeof(GLfloat));

  return GL_TRUE;
}

void _sgl_vector4f_print(SGLvector4f* v)
{
  static const char *templates[5] = {
    "%d:\t0, 0, 0, 1\n",
    "%d:\t%f, 0, 0, 1\n",
    "%d:\t%f, %f, 0, 1\n",
    "%d:\t%f, %f, %f, 1\n",
    "%d:\t%f, %f, %f, %f\n"
  };

  const char *t = templates[v->size];
  GLfloat *d = (GLfloat *)v->data;
  GLuint j, i = 0, count;

  printf("data-start\n");
  for (; d != v->start; STRIDE_F(d, v->stride), i++)
    printf(t, i, d[0], d[1], d[2], d[3]);

  printf("start-count(%u)\n", v->count);
}
