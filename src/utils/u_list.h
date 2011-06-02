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

/*
 * @brief Append <data> to the end of the list
 * @param data the data for the new element
 * @return the new start of the SList
 */
extern SList* s_list_append(SList* self, void* data);

/*
 * @brief Inserts a new element into the list at the given position.
 * @param data the data for the new element
 * @param position the position to insert the element, if position is out of
 * bound, the new element is append to the end of the list.
 * @return the new start of the SList
 */
extern SList* s_list_insert(SList* self, void* data, unsigned position);

/*
 * @brief Removes an element from a SList. If two elements contain the same
 * data, only the first is removed. If none of the elements contain the data,
 * the SList is unchanged.
 * @param data the data of the element to remove
 * @return  the new start of the SList
 */
extern SList* s_list_remove(SList* self, const void* data);

/*
 * @brief Removes an element from a SList, without freeing the element. The
 * removed element's prev and next links are set to NULL, so that it becomes a
 * self-contained list with one element.
 * @param an element in the Slist
 * @return new start of SList
 */
extern SList* s_list_remove_link(SList* self, SList* link);

/*
 * @brief Frees all of the memory used by a SList.
 */
extern void s_list_free(SList* self);

/*
 * @brief Returns the first element of the list
 */
extern SList* s_list_first(SList* self);

/*
 * @brief Returns the last element of the list
 */
extern SList* s_list_last(SList* self);

/*
 * @brief convenient macro fo getting the next element
 */
#define s_list_next(self) (self->next)

/*
 * @brief convenient macro fo getting the previous element
 */
#define s_list_previous(self) (self->prev)

/*
 * @brief Gets the element at the given position in a SList.
 * @param n the position of the element
 * @return the element, or NULL if the posistion is off the end of SList
 */
extern SList* s_list_nth(SList* self, unsigned n);

/*
 * @brief Gets the position of the element containing the given data (starting
 * from 0).
 * @param list the data to find
 * @return the index of the element containing the data, -1 if the data is not
 * found
 */
extern unsigned s_list_index(SList* self, const void* data);

/*
 * @brief Gets the position of the given element in the SList (starting from 0).
 * @param link an element in the SList
 * @return : the position of the element in the SList, or -1 if the element in
 * not found
 */
extern int s_list_position(SList* self, SList* link);

#endif /* __SGL_UTILS_LIST_H__ */
