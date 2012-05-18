/*
 * bm_list.c
 * Functions and macros for use with singly linked lists.
 *
 * Copyright (c) 2011-2012 Ben Morgan <neembi@googlemail.com>
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
#include <stdbool.h>

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
 *
 * @returns: pointer to an allocated list node.
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
 * list_empty: returns true if list is empty, false otherwise.
 *
 * @param *head: pointer to the head of the list.
 * @returns: true if list is empty, false otherwise.
 */
bool list_empty(struct list_node *head)
{
    return (head == NULL);
}

/*
 * list_length: returns the length of the list, 0 if empty.
 *
 * @param *head: pointer to the head of the list.
 * @returns: length of the list, 0 if empty.
 */
size_t list_length(struct list_node *head)
{
    size_t n = 0;

    while (head != NULL) {
        head = head->next;
        n++;
    }
    return n;
}

/*
 * list_to_array: return a NULL-terminated array with all the data pointers.
 *
 * @param *head: pointer to the head of the list.
 * @returns: pointer to an array of pointers to data. Only the last one will
 *           be NULL, so the size of the array may be smaller than the length
 *           of the list.
 */
void **list_to_array(struct list_node *head)
{
    struct list_node *iter;
    size_t count = 1;
    void **array;

    /* count how many non-NULL items there are */
    for (iter = head; iter != NULL; iter = head->next)
        if (iter->data != NULL)
            count++;

    /* add all non-NULL items to array and terminate with NULL */
    array = malloc(count * sizeof (void *));
    if (count > 1) {
        size_t index = 0;
        for (iter = head; iter != NULL; iter = head->next)
            if (iter->data != NULL)
                array[index++] = iter->data;
    }
    array[count] = NULL;

    return array;
}

/*
 * list_push: push a data on top of the list.
 *
 * @param **head: pointer to pointer to the head of the list; will be modified.
 * @param *data: data to push on top of the list, will replace previous head.
 */
void list_push(struct list_node **head, void *data)
{
    struct list_node *node = list_node();
    node->data = data;
    assert(head != NULL);

    node->next = *head;
    *head = node;
}

/*
 * list_pop: pop data from head and remove top node.
 *
 * @param **head: pointer to pointer to the head of the list; will be modified.
 * @returns: data stored in head, possibly NULL.
 */
void *list_pop(struct list_node **head)
{
    struct list_node *next;
    void *data;

    assert(head != NULL);
    if (list_empty(*head))
        return NULL;

    data = (*head)->data;
    next = (*head)->next;
    free(*head);
    *head = next;
    return data;
}

/*
 * list_insert: insert a list or a single node after head.
 * If head is an empty list, than node will replace head.
 *
 * @param **head: pointer to pointer to the node of the list where node should
 *                be inserted. head need not be the real head of a list.
 * @param *node: node to insert after head.
 */
void list_insert(struct list_node **head, struct list_node *node)
{
    struct list_node *tail;

    assert(head != NULL);
    if (list_empty(*head)) {
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
 * list_remove: remove and return the head node, replacing it with the next.
 *
 * @param **head: pointer to the head of the list; will be replaced with the
 *                next-in-line. Make sure nothing else points to this head.
 * @returns: pointer to the head list node, with next set to NULL.
 */
struct list_node *list_remove(struct list_node **head)
{
    struct list_node *top;
    struct list_node *next;

    assert(head != NULL);
    if (list_empty(*head))
        return NULL;

    next = (*head)->next;
    top = *head;
    top->next = NULL;
    *head = next;
    return top;
}

/*
 * list_free_nodes: free all the nodes (not the data) in the list.
 *
 * @param **head: pointer to pointer to the head of the list; will be modified.
 *                It will become NULL after all this has been done.
 */
void list_free_nodes(struct list_node **head)
{
    assert(head != NULL);
    while (*head != NULL) {
        struct list_node *temp = (*head)->next;
        free(*head);
        *head = temp;
    }
    *head = NULL;
}

/*
 * list_free_all: free all the nodes AND the data in the list.
 *
 * @param **head: pointer to pointer to the head of the list; will be modified.
 *                It will become NULL after all this has been done.
 */
void list_free_all(struct list_node **head)
{
    assert(head != NULL);
    while (*head != NULL) {
        struct list_node *temp;
        if ((*head)->data != NULL)
            free((*head)->data);
        temp = (*head)->next;
        free(*head);
        *head = temp;
    }
    *head = NULL;
}

/* vim: set cin ts=4 sw=4 et: */
