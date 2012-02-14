/*
 * bm_list.c
 * Functions and macros for use with singly linked lists.
 * 
 * Copyright (c) 2011 Ben Morgan <neembi@googlemail.com>
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "bm_list.h"
#include <stdlib.h>

/*
 * struct list_node is defined by the user of this library.
 * See bm_list.h for more information on that.
 */
struct list_node {
    void *data;
    struct list_node *next;
};


/*
 * list_node: create a new list node using malloc.
 */
struct list_node *list_node()
{
    struct list_node *ptr;

    ptr = malloc(sizeof (struct list_node));
    ptr->data = NULL;
    ptr->next = NULL;
    return ptr;
}

/*
 * list_empty: returns 1 if list is empty, 0 otherwise.
 * This ought to be implemented as a macro.
 */
int list_empty(struct list_node *head)
{
    return (head == NULL);
}

/*
 * list_length: returns the length of the list, 0 if empty.
 * Calling this function should be avoided, as it works in
 * time O(n).
 */
int list_length(struct list_node *head)
{
    int n = 0;

    while (head != NULL) {
        head = head->next;
        n++;
    }
    return n;
}

/*
 * list_push: push a node or a list of nodes onto the top of a list.
 * Returns new head of the list, which is node.
 */
struct list_node *list_push(struct list_node *head, struct list_node *node)
{
    struct list_node *top;
    
    top = node;
    while (node->next != NULL)
        node = node->next;
    node->next = head;
    return top;
}

/*
 * list_pop: pop a node from the top of the list into data.
 * Returns new head of the list.
 */
struct list_node *list_pop(struct list_node *head, void **data)
{
    if (list_empty(head)) {
        *data = NULL;
        return head;
    }

    *data = head->data;
    return head->next;
}
        

/*
 * list_insert: insert a list or a single node into another list.
 * May modify head, if head == NULL.
 */
void list_insert(struct list_node **head, struct list_node *node)
{
    struct list_node *tail;
    
    if (*head == NULL) {
        *head = node;
        return;
    }

    tail = (*head)->next;
    (*head)->next = node;
    if (tail != NULL) {
        while (node->next != NULL)
            node = node->next;
        node->next = tail;
    }
}

/*
 * list_remove: remove the head node, replacing it with next node,
 * and return a pointer to the data. Note that the node is
 * automatically freed, but the data is not. It is possible that
 * a NULL pointer is returned.
 */
void *list_remove(struct list_node **head)
{
    void *dataptr;
    struct list_node *next;

    if (*head == NULL)
        return NULL;

    dataptr = (*head)->data;
    next = (*head)->next;
    free(*head);
    *head = next;
    return dataptr;
}

/*
 * list_free_all: assuming that the nodes and the data are
 * allocated using malloc, free all that memory. Note
 * that *head becomes invalid.
 */
void list_free_all(struct list_node *head)
{
    struct list_node *temp;
    while (head != NULL) {
        if (head->data != NULL)
            free(head->data);
        temp = head->next;
        free(head);
        head = temp;
    }
}

/*
 * list_free_nodes: assuming that the nodes are allocated using
 * malloc, free all that memory. Note that *head becomes invalid.
 */
void list_free_nodes(struct list_node *head)
{
    struct list_node *temp;
    while (head != NULL) {
        temp = head->next;
        free(head);
        head = temp;
    }
}


/* vim: set cin ts=4 sw=4 et: */
