/*
 * bm_util.h
 * Various helpful functions.
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

#ifndef BMUTIL_H
#define BMUTIL_H


#define MAX_OUT_LINE 80


typedef struct list_node {
    char *data;
    struct list_node *next;
} Node;


/*
 * get_regex_files: get all files in dir that match regex.
 * A pointer to the head of a linked list is returned.
 * Warning: make sure to call list_free_all() on the returned Node when done.
 */
Node *get_regex_files(const char * /*regex*/, const char * /*dir*/);

/*
 * list_files: print a listing of all the files in linked list.
 */
void list_files(Node * /*head*/);

/*
 * list_strjoin: join all the list elements together.
 * Returns the joined string. Don't forget to free it when done.
 */
char *list_strjoin(Node * /*head*/, const char * /*sep*/);


#endif // BMUTIL_H
