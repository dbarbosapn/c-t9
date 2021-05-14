#include <linked_list_ext.h>

/**
 * Adds a node in order to the list using a given integer hashtable
 * Returns the head.
 **/
Node *list_add_inorder_ht(Node *head, void *value, size_t alloc_size,
                          HashTable *ht) {
    Node *node = create_node(value, alloc_size);

    if (head == NULL) {
        return node;
    }

    // Edge case where the new node should be the new head
    if (hashtable_get(ht, node->value) - hashtable_get(ht, head->value) > 0) {
        node->next = head;
        return node;
    }

    Node *prev = head;
    Node *curr = head->next;

    while (curr != NULL && (hashtable_get(ht, node->value) -
                            hashtable_get(ht, curr->value)) <= 0) {
        prev = curr;
        curr = curr->next;
    }

    prev->next = node;
    node->next = curr;

    return head;
}