/*
 * bm_list.h
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

#ifndef BMLIST_H
#define BMLIST_H

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
 */
extern struct list_node *list_node();

/*
 * list_empty: returns 1 if list is empty, 0 otherwise.
 *
 * @param *head: pointer to the head of the list.
 * @returns: 1 if list is empty, 0 otherwise.
 */
extern int list_empty(struct list_node * /*head*/);

/*
 * list_length: returns the length of the list, 0 if empty.
 * Calling this function should be avoided, as it works in
 * time O(n).
 *
 * @param *head: pointer to the head of the list.
 * @returns: length of the list, 0 if empty.
 */
extern int list_length(struct list_node * /*head*/);

/*
 * list_push: push a node or a list of nodes onto the top of a list.
 *
 * @param *head: pointer to the head of the list.
 * @param *node: pointer to node or list of nodes to push ontop of head.
 * @returns: pointer to new head of the list, which is node.
 */
extern struct list_node *list_push(struct list_node * /*head*/, struct list_node * /*node*/);

/*
 * list_pop: pop a node from the top of the list into data.
 * @returns: pointer to new head of the list.
 */
extern struct list_node *list_pop(struct list_node * /*head*/, void ** /*data*/);

/*
 * list_insert: insert a list or a single node into another list.
 * May modify head, if head == NULL.
 */
extern void list_insert(struct list_node ** /*head*/, struct list_node * /*node*/);

/*
 * list_remove: remove the head node, replacing it with next node,
 * and return a pointer to the data. Note that the node is
 * automatically freed, but the data is not. It is possible that
 * a NULL pointer is returned.
 */
extern void *list_remove(struct list_node ** /*head*/);

/*
 * list_free_all: assuming that the nodes and the data are
 * allocated using malloc, free all that memory. Note
 * that *head becomes invalid.
 */
extern void list_free_all(struct list_node * /*head*/);

/*
 * list_free_nodes: assuming that the nodes are allocated using
 * malloc, free all that memory. Note that *head becomes invalid.
 */
extern void list_free_nodes(struct list_node * /*head*/);

#endif /* BMLIST_H */
