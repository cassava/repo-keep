/*
 * libcassava/list_str.h
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
 * Functions and macros for use with singly linked lists
 * that use strings as the data type in the struct list_node.
 *
 * This is in accordance to the requirements in list.h.
 *
 * \author Ben Morgan
 * \date 2011-2012
 */

#ifndef LIBCASSAVA_LIST_STR_H
#define LIBCASSAVA_LIST_STR_H

#include <regex.h>
#include <stdbool.h>


/**
 * Since list_node is required to contain data of type \c char*,
 * we define it explicitely here.
 */
typedef struct list_node {
    char *data;
    struct list_node *next;
} NodeStr;

/**
 * Print all the data in a linked list.
 * Behavior is undefined if either parameter is \c NULL.
 *
 * \param head Head of a linked list.
 * \param sep  String which separates each entry from the next.
 */
extern void list_print(const NodeStr *head, const char *sep);

/**
 * Print all the data in a linked list, each entry on its own line.
 * Behavior is undefined if either parameter is NULL.
 *
 * \param head   Head of a linked list.
 * \param prefix String which prefixes each entry.
 */
extern void list_println(const NodeStr *head, const char *prefix);

/**
 * Search for a string in the list, return if found, otherwise \c NULL.
 * Behavior is undefined if either parameter is NULL.
 *
 * \param haystack Head of a linked list.
 * \param needle   String to search for in list.
 * \return Pointer to NodeStr containing \a needle, or \c NULL.
 */
extern const NodeStr *list_search(const NodeStr *haystack, const char *needle);

/**
 * Join all the list elements together.
 * Behavior is undefined if either parameter is \c NULL.
 *
 * \param head Head of a linked list.
 * \param sep  Separator between the joined strings.
 * \return Newly allocated string (don't forget to free() it).
 */
extern char *list_strjoin(const NodeStr *head, const char *sep);

/**
 * Arguments for the filter_regex function, which need to be passed
 * along with list_filter.
 *
 * \see filter_regex
 */
struct filter_regex_args {
    regex_t preg;
};

/**
 * A filter function for list_filter, to keep all nodes satisfying a regular expression.
 *
 * \param string    String to match to (the victim).
 * \param arguments Pointer to a struct filter_regex_args with a compiled regular
 *                  expression.
 * \return True if \a filepath matches the regex compiled in \a arguments->preg.
 *
 * \b Example:
 * \code
 *     NodeStr *head;
 *     struct filter_regex_args args;
 *     int errcode = regcomp(&args.preg, regex, REG_EXTENDED);
 *     int retval = list_filter(&head, filter_regex, (void *)&args);
 *     regfree(&args.preg);
 *     list_free_all(head);
 * \endcode
 */
extern bool filter_regex(void *string, void *arguments);

/**
 * Filter nodes in head with regex; only nodes matching regex are allowed.
 * Behavior is undefined if \a regex is \c NULL.
 *
 * \param head  Head of a linked list.
 * \param regex Regular expression string which nodes must match to remain in the list.
 * \return count of nodes that match regex, -1 if error.
 *
 * \b WARNING: nodes that do NOT match regex are completely freed: node and data.
 */
extern int list_filter_regex(NodeStr **head, const char *regex);

#endif /* LIBCASSAVA_LIST_STR_H */
