/*
 * libcassava/util.h
 * Various helpful functions.
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

#ifndef LIBCASSAVA_UTIL_H
#define LIBCASSAVA_UTIL_H

#include "list_str.h"

#include <sys/types.h>


/*
 * get_regex_files: get all files in dir that match regex.
 * Returns: number of files found.
 * Warning: make sure to call list_free_all() on the head Node when done.
 */
extern int get_regex_files(const char * /*regex*/, const char * /*dir*/, NodeStr ** /*head*/);

/*
 * get_younger_files: get all files in dir that are younger than age.
 * Returns: number of files found.
 * Warning: make sure to call list_free_all() on the head Node when done.
 */
extern int get_younger_files(const time_t /*age*/, const char * /*dir*/, NodeStr ** /*head*/);

/*
 * TODO: Implement me!
 */
extern void print_columns(char ** /*array*/);

#endif /* LIBCASSAVA_UTIL_H */

/* vim: set cin ts=4 sw=4 et: */
