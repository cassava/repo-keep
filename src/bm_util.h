/*
 * bm_util.h
 * Various helpful functions.
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

#ifndef BMUTIL_H
#define BMUTIL_H

#include <sys/types.h>

#define MAX_OUT_LINE 80


/* See bm_list.h for an explanation on this. */
typedef struct list_node {
    char *data;
    struct list_node *next;
} Node;


/*
 * get_regex_files: get all files in dir that match regex.
 * Returns: number of files found.
 * Warning: make sure to call list_free_all() on the head Node when done.
 */
extern int get_regex_files(const char * /*regex*/, const char * /*dir*/, Node ** /*head*/);

/*
 * get_younger_files: get all files in dir that are younger than age.
 * Returns: number of files found.
 * Warning: make sure to call list_free_all() on the head Node when done.
 */
extern int get_younger_files(const time_t /*age*/, const char * /*dir*/, Node ** /*head*/);

/*
 * TODO: Implement me!
 */
extern void print_columns(char ** /*array*/);

/*
 * list_filter_destroy: filter nodes in head with regex; only nodes matching regex are allowed.
 * Returns: count of nodes that match regex, -1 if error.
 * Warning: nodes that do NOT match regex are completely freed: node and data.
 */
extern int list_filter_destroy(const char * /*regex*/, Node ** /*head*/);

/*
 * list_files: print a listing of all the files in linked list.
 */
extern void list_files(Node * /*head*/, char * /*prefix*/);

/*
 * list_search: return Node containing data, or NULL.
 */
extern Node *list_search(char * /*search*/, Node * /*head*/);

/*
 * list_strjoin: join all the list elements together.
 * Returns the joined string. Don't forget to free it when done.
 */
extern char *list_strjoin(Node * /*head*/, const char * /*sep*/);


#endif // BMUTIL_H

/* vim: set cin ts=4 sw=4 et: */
