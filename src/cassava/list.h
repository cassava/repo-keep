/*
 * libcassava/list.h
 * Functions and macros for use with singly linked lists.
 */

/*
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

#ifndef LIBCASSAVA_LIST_H
#define LIBCASSAVA_LIST_H

#include <stdbool.h>
#include <stdlib.h>

/*
 * In order to use these functions, you must define
 *
 *     struct list_node {
 *         void *data;
 *         struct list_node *next;
 *     }
 *
 * with the appropriate datatype for data.
 */

/*
 * list_node: create a new list node using malloc.
 *
 * @returns: pointer to an allocated list node.
 *
 * The node returned by this function constitutes a list of length 1.
 * For safety, data and next of the node are initialized to NULL.
 */
extern struct list_node *list_node();

/*
 * list_empty: returns true if list is empty, false otherwise.
 *
 * @param *head: pointer to the head of the list.
 * @returns: true if list is empty, false otherwise.
 *
 * When is a list empty? Since we can't know if the data of a node is
 * meaningful or not, we do not consider the data, not even for NULL.
 * An empty list is a list where head is a NULL-pointer.
 */
extern bool list_empty(struct list_node * /*head*/);

/*
 * list_length: returns the length of the list, 0 if empty.
 *
 * @param *head: pointer to the head of the list.
 * @returns: length of the list, 0 if empty.
 *
 * Calling this function should be avoided, as it works in time O(n).
 */
extern size_t list_length(struct list_node * /*head*/);

/*
 * list_to_array: return a NULL-terminated array with all the data pointers.
 *
 * @param *head: pointer to the head of the list.
 * @returns: pointer to an array of pointers to data. Only the last one will
 *           be NULL, so the size of the array may be smaller than the length
 *           of the list.
 *
 * Note: the returned array must be freed.
 */
extern void **list_to_array(struct list_node * /*head*/);

/*
 * list_push: push a data on top of the list.
 *
 * @param **head: pointer to pointer to the head of the list; will be modified.
 * @param *data: data to push on top of the list, will replace previous head.
 *
 * A new node will be created with list_node() to hold data.
 * WARNING: Beware of pushing non-allocated data onto the list,
 * as list_free_all() will try to free the data.
 */
extern void list_push(struct list_node ** /*head*/, void * /*data*/);

/*
 * list_pop: pop data from head and remove top node.
 *
 * @param **head: pointer to pointer to the head of the list; will be modified.
 * @returns: data stored in head, possibly NULL.
 *
 * The head node will be replaced by the next-in-line, and will be freed.
 * If list is empty, then NULL will be returned, however a NULL returned does
 * not mean that the list is empty! A node may also store data that is NULL.
 */
extern void *list_pop(struct list_node ** /*head*/);

/*
 * list_insert: insert a list or a single node after head.
 * If head is an empty list, than node will replace head.
 *
 * @param **head: pointer to pointer to the node of the list where node should
 *                be inserted. head need not be the real head of a list.
 * @param *node: node to insert after head.
 */
extern void list_insert(struct list_node ** /*head*/, struct list_node * /*node*/);

/*
 * list_remove: remove and return the head node, replacing it with the next.
 *
 * @param **head: pointer to the head of the list; will be replaced with the
 *                next-in-line. Make sure nothing else points to this head.
 * @returns: pointer to the head list node, with next set to NULL.
 *
 * WARNING: you are just asking for trouble if you remove a node which is not
 * the real head of the list! So don't do it.
 */
extern struct list_node *list_remove(struct list_node ** /*head*/);

/*
 * list_free_nodes: free all the nodes (not the data) in the list.
 *
 * @param **head: pointer to pointer to the head of the list; will be modified.
 *                It will become NULL after all this has been done.
 *
 * We assume that all the nodes have been allocated using malloc.
 */
extern void list_free_nodes(struct list_node ** /*head*/);

/*
 * list_free_all: free all the nodes AND the data in the list.
 *
 * @param **head: pointer to pointer to the head of the list; will be modified.
 *                It will become NULL after all this has been done.
 *
 * We assume that all the nodes AND the data have been allocated using malloc.
 */
extern void list_free_all(struct list_node ** /*head*/);

#endif /* LIBCASSAVA_LIST_H */
/* vim: set cin ts=4 sw=4 et: */
