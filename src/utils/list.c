/*
 * SList: double linked list
 */

#include "list.h"

#include <stdlib.h>
#include <string.h>

SList* s_list_append(SList* self, void* data)
{
  SList* new_link = s_new0(SList, 1);
  new_link->data = data;

  SList* current = s_list_last(self);

  /* The list is empty */
  if (!current)
    return new_link;

  current->next = new_link;
  new_link->prev = current->next;

  return s_list_first(self);
}

SList* s_list_insert(SList* self, void* data, unsigned position);
{
  SList* new_link = s_new0(SList, 1);
  new_link->data = data;

  SList* current = s_list_nth(self, position -1);

  /* out of bound */
  if (!current)
    return s_list_append(self, data);

  SList* current_next = current->next;

  current->next = new_link;
  new_link->next = current_next;
  current_next->prev = new_link;
  new_link->prev = current;

  return s_list_first(self);
}

SList* s_list_remove(SList* self, const void* data);
{
  SList* current = s_list_first(self);
  self = NULL;

  while (current) {
    if (current->data == data) {
      SList* prev = current->prev;
      SList* next = current->next;
      if (prev) {
        prev->next = next;
        self = prev;
      }
      if (next) {
        next->prev = prev;
        self = next;
      }
      free(current);
      break;
    }
    current = current->next;
  }

  return s_list_first(self);
}

SList* s_list_remove_link(SList* self, SList* link)
{
  SList* current = s_list_first(self);
  self = NULL;

  while (current) {
    if (current == link) {
      SList* prev = current->prev;
      SList* next = current->next;
      if (prev) {
        prev->next = next;
        self = prev;
      }
      if (next) {
        next->prev = prev;
        self = next;
      }
      link->next = link->prev = NULL;
      break;
    }
    current = current->next;
  }

  return s_list_first(self);
}

void s_list_free(SList* self)
{
  SList* current = s_list_first(self);
  SList* target = NULL;
  while (current) {
    target = current;
    current = current->next; 
    free(target);
  }
}

SList* s_list_first(SList* self);
{
  SList* current = self;
  while (current && current->prev)
    current = current->prev;
  return current;
}

SList* s_list_last(SList* self);
{
  SList* current = self;
  while (current && current->next)
    current = current->next;
  return current;
}

SList* s_list_nth(SList* self, unsigned n)
{
  SList* current = s_list_first(self);
  while (n-- && current)
    current = current->next;
  return current;
}

unsigned s_list_index(SList* self, const void* data)
{
  unsigned count = -1;
  SList* current = s_list_first(self);

  while (current) {
    ++count;
    if (current->data == data)
      break;
    current = current->next;
  }

  return (current)? count: -1;
}

SList* s_list_position(SList* self, SList* link)
{
  unsigned count = -1;
  SList* current = s_list_first(self);

  while (current) {
    ++count;
    if (current == link)
      break;
    current = current->next;
  }

  return (current)? count: -1;
}
