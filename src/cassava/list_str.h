/*
 * libcassava/list_str.h
 * Functions and macros for use with singly linked lists
 * that use as data in the list_node struct char *.
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

#ifndef LIBCASSAVA_LIST_STR_H
#define LIBCASSAVA_LIST_STR_H

#include <stdbool.h>

/* Maximum error line length printed by regex errors. */
#define MAX_ERROR_LINE_LENGTH 80


/*
 * Since list_node is required to contain data of type char *,
 * we define it explicitely here.
 */
typedef struct list_node {
    char *data;
    struct list_node *next;
} NodeStr;

/*
 * list_print: print all the data in a linked list.
 *
 * @param *head: head of a linked list.
 * @param *sep: string which separates each entry from the next.
 *
 * Behavior is undefined if either parameter is NULL.
 */
extern void list_print(NodeStr * /*head*/, const char * /*sep*/);

/*
 * list_print: print all the data in a linked list, each entry on its own line.
 *
 * @param *head: head of a linked list.
 * @param *prefix: string which prefixes each entry.
 *
 * Behavior is undefined if either parameter is NULL.
 */
extern void list_println(NodeStr * /*head*/, const char * /*prefix*/);

/*
 * list_print: print all the data in a linked list, separated by sep.
 *
 * @param *head: head of a linked list.
 * @param *sep: string which separates all entries.
 *
 * Behavior is undefined if either parameter is NULL.
 */
extern bool list_contains(NodeStr * /*haystack*/, char * /*needle*/);

/*
 * list_search:
 *
 * @param *haystack:
 * @param *needle:
 * @returns: pointer to NodeStr containing needle, or NULL.
 *
 * Behavior is undefined if either parameter is NULL.
 */
extern NodeStr *list_search(NodeStr * /*haystack*/, const char * /*needle*/);

/*
 * list_strjoin: join all the list elements together.
 * @returns: newly allocated string (don't forget to free() it).
 *
 * Behavior is undefined if either parameter is NULL.
 */
extern char *list_strjoin(NodeStr * /*head*/, const char * /*sep*/);

/*
 * list_filter_destroy: filter nodes in head with regex; only nodes matching regex are allowed.
 * Returns: count of nodes that match regex, -1 if error.
 * Warning: nodes that do NOT match regex are completely freed: node and data.
 *
 * Behavior is undefined if either parameter is NULL.
 */
extern int list_filter_destroy(NodeStr ** /*head*/, const char * /*regex*/);

#endif /* LIBCASSAVA_LIST_STR_H */
/* vim: set cin ts=4 sw=4 et: */
