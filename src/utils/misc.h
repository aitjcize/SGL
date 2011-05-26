#ifndef __SGL_UTILS_MISC_H__
#define __SGL_UTILS_MISC_H__

#include <stdlib.h>
#include <string.h>

#define s_new0(struct_type, count) \
  (struct_type*) (__extension__({                            \
    int   __s = sizeof(struct_type) * count;                 \
    void* __m = malloc(sizeof(struct_type) * count);         \
    memset(__m, 0, __s);                                     \
    __m;                                                     \
  }))

#endif /* __SGL_UTILS_MISC_H__ */
