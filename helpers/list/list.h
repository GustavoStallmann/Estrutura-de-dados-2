#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef void* List; 
typedef void* ListNode; 
typedef int ListValue; 

/* 
 * Create a new linked list instance
 * @return `List` The new created list
 */
List* new_list();

/* 
 * Insert a new value on a given list
 * @param `list` the list to insert the value
 * @param `value` the value to be inserted
 * @return `list` the given list
 */
List* list_insert(List *list, ListValue value);

/* 
 * Remove the last value of a list
 * @param `list` the list to remove the value
 * @return `list` the given list
*/
List* list_pop(List *list);

/* 
 * Search for a value in the given list
 * @param `list` the list to search the value
 * @param `value` the value to be searched 
 * @return bool: true if the value was found, false otherwise
 */
bool list_search(List *list, ListValue value);


/* 
 * Get the amount of elements in the given list
 * @param `list` the list to get the size
 * @return int: the amount of elements
*/
int list_get_size(List *list); 

#endif