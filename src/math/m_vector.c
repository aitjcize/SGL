/*
 * Vector Math Library
 *
 * Most of code in this file is taken from Mesa
 *
 */

#include "m_vector.h"
#include "utils.h"

void _math_vector4f_init(SGLvector4f *v, GLbitfield flags,
                         GLfloat (*storage)[4])
{
  v->stride = 4 * sizeof(GLfloat);
  v->size = 2;
  v->data = storage;
  v->start = (GLfloat*) storage;
  v->count = 0;
}

void _math_vector4f_alloc(SGLvector4f *v, GLbitfield flags, GLuint count)
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

void _math_vector4f_alloc_double(SGLvector4f *v)
{
  if (v->flags & VEC_MALLOC) {
    unsigned offset = (void*)v->start - v->storage;
    void* new_storage = malloc(v->storage_count * 2 * 4 * sizeof(GLfloat));
    memcpy(new_storage, v->storage, v->storage_count * 4 * sizeof(GLfloat));
    free(v->storage);
    v->storage = new_storage;
    v->storage_count *= 2;
    v->data = (GLfloat (*)[4]) v->storage;
    v->start = v->storage + offset;
  }
}

void _math_vector4f_free(SGLvector4f* v)
{
  if (v->flags & VEC_MALLOC) {
    free(v->storage);
    v->data = NULL;
    v->start = NULL;
    v->storage = NULL;
    v->flags &= ~VEC_MALLOC;
  }
}

GLboolean _math_vector4f_push_back(SGLvector4f* v, GLfloat* data, GLuint size)
{
  if (v->flags & VEC_MALLOC) {
    if ((void*)v->start >= v->storage + v->storage_count * v->stride)
      _math_vector4f_alloc_double(v);
  }

  memcpy(v->start, data, size * sizeof(GLfloat));
  v->count++;
  v->start = (GLfloat*) (v->data + v->count);

  return GL_TRUE;
}

GLboolean _math_vector4f_pop_back(SGLvector4f* v, GLfloat* data, GLuint size)
{
  if (v->count == 0)
    return GL_FALSE;

  v->count--;
  v->start = (GLfloat*) (v->data + v->count);

  if (data)
    memcpy(data, v->start, size * sizeof(GLfloat));

  return GL_TRUE;
}

GLboolean _math_vector4f_empty(SGLvector4f* v)
{
  return (v->count == 0);
}

void _math_vector4f_print(SGLvector4f* v)
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
  GLuint i = 0;

  printf("%p: data-start\n", v);
  for (; d != v->start; d = (GLfloat*)((GLubyte *)d + v->stride), i++)
    printf(t, i, d[0], d[1], d[2], d[3]);

  printf("start-count(%u)\n", v->count);
}
