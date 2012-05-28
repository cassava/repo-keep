/*
 * libcassava/list.h
 * vim: set cin ts=4 sw=4 et:
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

/**
 * \file
 * Functions and macros for use with singly-linked lists.
 *
 * In order to use these functions, you must define
 * \code
 *     struct list_node {
 *         void *data;
 *         struct list_node *next;
 *     }
 * \endcode
 * with the appropriate datatype for \a data.
 *
 * \author Ben Morgan
 * \date 28. May 2012
 */

#ifndef LIBCASSAVA_LIST_H
#define LIBCASSAVA_LIST_H

#include <stdbool.h>
#include <stdlib.h>


/**
 * \struct list_node
 *
 * The head of a singly-linked list and at the same time the node.  If \a data
 * is \c NULL, then a node is considered to hold nothing, and if \a next is \c
 * NULL, then the node is considere the last in the list. A pointer to struct
 * list_node which is \c NULL is considered an empty linked list.
 *
 * \param data Pointer to data that the node ``stores''.
 * \param next Pointer to the next node in the linked list.
 *
 * \warning Most functions assume that if there is a pointer unequal to \c
 * NULL, that then \a data is also not \c NULL; therefore make sure you don't
 * have useless nodes lying around, they might bite you in the end.
 */

/**
 * Create a new list node using malloc.
 *
 * The node returned by this function constitutes a list of length 1.
 * For safety, \a data and \a next of the node are initialized to \c NULL.
 *
 * \return Pointer to an allocated list node.
 */
extern struct list_node *list_node();

/**
 * Returns true if list is empty, false otherwise.
 *
 * When is a list empty? Since we can't know if the data of a node is
 * meaningful or not, we do not consider the data, not even for \c NULL.
 * An empty list is a list where head is a null pointer.
 *
 * \param head Pointer to the head of the list.
 * \return true if list is empty, false otherwise.
 */
extern bool list_empty(const struct list_node *head);

/**
 * Returns the length of the list, 0 if empty.
 *
 * \param head Pointer to the head of the list.
 * \return Length of the list, 0 if empty.
 *
 * \note Calling this function should be avoided, as it works in time O(n).
 */
extern size_t list_length(const struct list_node *head);

/**
 * Returns a NULL-terminated array with all the data pointers.
 * The \a output array must be freed.
 *
 * \param head   Pointer to the head of the list.
 * \param output Pointer to a string array. Afterwards the pointer will point
 *               to a newly allocated array, which needs to be freed. The last
 *               entry in the array has the pointer entry \c NULL.
 * \return Number of elements (without the last \c NULL element) in the returned
 *         array.
 *
 * \b Example:
 * \code
 *     char **array;
 *     list_to_array(head, &output);
 *     free(output);
 * \endcode
 */
extern size_t list_to_array(const struct list_node *head, void ***output);

/**
 * Push a data on top of the list.
 *
 * A new node will be created with list_node() to hold data.
 *
 * \param head Pointer to pointer to the head of the list; will be modified.
 * \param data Data to push on top of the list, will replace previous head.
 *
 * \warning Beware of pushing non-dynamic allocated data onto the list,
 * as list_free_all() will try to free the data.
 */
extern void list_push(struct list_node **head, void *data);

/**
 * Pop data from head and remove top node.
 *
 * The head node will be replaced by the next-in-line, and will be freed.
 * If list is empty, then \c NULL will be returned, however a \c NULL returned does
 * not mean that the list is empty! A node may also store data that is \c NULL.
 *
 * \param head Pointer to pointer to the head of the list; will be modified.
 * \return Data stored in head, possibly \c NULL.
 */
extern void *list_pop(struct list_node **head);

/**
 * Insert a list or a single node after head.
 * If head is an empty list, than node will replace head.
 *
 * \param head Pointer to pointer to the node of the list where node should
 *             be inserted. \a head need not be the real head of a list.
 * \param node Node to insert after \a head.
 */
extern void list_insert(struct list_node **head, struct list_node *node);

/**
 * Remove and return the head node, replacing it with the next.
 *
 * \param head Pointer to the head of the list; will be replaced with the
 *             next-in-line. Make sure nothing else points to this \a head.
 * \return Pointer to the head list node, with \a next set to \c NULL.
 *
 * \warning you are just asking for trouble if you remove a node which is not
 * the real head of the list! So don't do it.
 */
extern struct list_node *list_remove(struct list_node **head);

/**
 * Remove all nodes from the list where the function \a filter() does not
 * return \c true.
 *
 * \note Any element which will be excluded from the list will be freed,
 * as if it were (both node and data) allocated with \a malloc().
 *
 * \param head   Pointer to the head of the list; the list will be altered,
 *               and may be \c NULL.
 * \param filter Function which takes the type \a list_node::data and
 *               returns true if it should stay in the list. The argument
 *               to filter can also be a non-void datatype.
 * \return The number of elements which matched filter.
 *
 * \b Example: Without arguments:
 * \code
 *     bool filter_notempty(char *str, void *)
 *     {
 *         return strlen(str) > 0;
 *     }
 *
 *     int main()
 *     {
 *         NodeStr *head = get_some_dynamic_list();
 *         list_filter(&head, filter_notempty, NULL);
 *         list_free_all(&head);
 *         return 0;
 *     }
 * \endcode
 *
 * \b Example: With arguments:
 * \code
 *     struct filter_comp_args {
 *         int number;
 *         int comp;
 *     }
 *
 *     bool filter_comp(int *element, struct filter_comp_args *arguments)
 *     {
 *         int comp = arguments->comp;
 *         if (comp < 0)
 *             return *element < arguments->number;
 *         else if (comp > 0)
 *             return *element > arguments->number;
 *         else
 *             return *element == arguments->number;
 *     }
 *
 *     int main()
 *     {
 *         // Ficticious code, but let's imagine it would work:
 *         int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
 *         NodeStr *head = list_from_basic_array(array);
 *         struct filter_comp_args arguments = {5, -1};
 *         int retval = list_filter(head, filter_comp, &arguments);
 *         return 0;
 *     }
 * \endcode
 */
extern size_t list_filter(struct list_node **head,
                          bool (*filter)(void *, void *),
                          void *arguments
                          );

/**
 * Free all the nodes (not the data) in the list.
 * We assume that all the nodes have been allocated using malloc().
 *
 * \param head Pointer to pointer to the head of the list; will be modified.
 *             It will become \c NULL after all this has been done.
 */
extern void list_free_nodes(struct list_node **head);

/**
 * Free all the nodes AND the data in the list.
 * We assume that all the nodes AND the data have been allocated using malloc().
 *
 * \param head Pointer to pointer to the head of the list; will be modified.
 *             It will become \c NULL after all this has been done.
 */
extern void list_free_all(struct list_node **head);

#endif /* LIBCASSAVA_LIST_H */
