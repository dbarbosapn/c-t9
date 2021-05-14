#ifndef _LINKED_LIST_EXT
#define _LINKED_LIST_EXT

#include <int_hashtable.h>
#include <linked_list.h>

Node *list_add_inorder_ht(Node *head, void *value, size_t alloc_size,
                          HashTable *ht);

#endif