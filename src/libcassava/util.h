/*
 * libcassava/util.h
 * vim: set cin ts=4 sw=4 et cc=81:
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
 * Various utility functions that make use of libcassava.
 *
 * In order to use this header file, your library must have the ceil() function
 * available. On some systems, you need the \a libm math library.
 *
 * \author Ben Morgan
 * \date 1. June 2012
 */

#ifndef LIBCASSAVA_UTIL_H
#define LIBCASSAVA_UTIL_H

#include <stdlib.h>

/** Define the width separating two columns for the function print_columns(). */
#define PRINT_COLUMNS_SEP_WIDTH 2

/**
 * Print an array of strings in columns to STDOUT, like the *nix command \e ls
 * does it.
 *
 * \param array Array of strings.
 * \param len   Length of the array, or number of entries to read.  So if \code
 *              len = 5 \endcode then \a array will be read till \c array[4],
 *              which makes five elements.
 *
 * TODO Make this \a array \c const.
 */
extern void print_columns(char **array, size_t len);

#endif /* LIBCASSAVA_UTIL_H */
