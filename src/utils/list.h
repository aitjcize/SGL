/*
 * SList: double linked list
 */

#ifndef __SGL_UTILS_LIST_H__
#define __SGL_UTILS_LIST_H__

typedef struct _SList SList;

struct _SList {
  void* data;
  SList *next;
  SList *prev;
};

extern SList* s_list_append(SList* self, void* data);

extern SList* s_list_insert(SList* self, void* data, unsigned position);

extern SList* s_list_remove(SList* self, const void* data);

extern SList* s_list_remove_link(SList* self, SList* link);

extern void s_list_free(SList* self);

extern SList* s_list_first(SList* self);

extern SList* s_list_last(SList* self);

#define s_list_next(self) (self->next)

#define s_list_previous(self) (self->prev)

extern SList* s_list_nth(SList* self, unsigned n);

extern unsigned s_list_index(SList* self, const void* data);

extern SList* s_list_position(SList* self, SList* link);

#endif /* __SGL_UTILS_LIST_H__ */
