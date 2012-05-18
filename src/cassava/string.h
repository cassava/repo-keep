/*
 * libcassava/string.h
 * Various helpful string functions.
 *
 * Note: the function names are all prefixed with cs_ to avoid collisions with
 * other functions in string.h or other libraries. Think cassava string.
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

#ifndef LIBCASSAVA_STRING_H
#define LIBCASSAVA_STRING_H

#include <stdbool.h>

/*
 * cs_strcat: concatenates two strings.
 * Returns: may be NULL, if no space can be allocated.
 * Note: remember to call free() on result of this function.
 */
extern char *cs_strcat(const char * /*f1*/, const char * /*f2*/);

/*
 * cs_strjoin: concatenates to a string n elements from an array,
 * using the third argument as the delimiter between elements.
 * Arguments:
 *   extra = additional space allocated which will be unused.
 * Returns: may be NULL, if no space can be allocated.
 * Note: remember to call free() on result of this function.
 */
extern char *cs_strjoin(char ** /*arr*/, int /*n*/, const char * /*sep*/, int /*extra*/);

/*
 * cs_stracat: concatenates to a string n elements from an array.
 * Arguments: the first argument may be a null pointer.
 *   extra = addional space allocated which will be unused.
 * Returns: may be NULL, if no space can be allocated.
 * Note: remember to call free() on result of this function.
 */
extern char *cs_stracat(const char * /*str1*/, char ** /*arr*/, int /*n*/, int /*extra*/);

/*
 * cs_strvcat: concatenates variable amount of strings into a new string.
 * Arguments: takes char * arguments with a mandatory trailing NULL argument.
 * Returns: may be NULL, if no space can be allocated.
 * Note: remember to call free() on result of this function.
 */
extern char *cs_strvcat(const char * /*f1*/, ...);

/*
 * cs_substr: get a substring from a string.
 */
extern char *cs_substr(const char * /*input*/, unsigned /*start_offset*/, unsigned /*end_offset*/);

/*
 * cs_isprefix: compare two strings, return true if first is prefix of second.
 * Behaviour is undefined if either of the parameters are NULL.
 *
 * @param *child: child must be a prefix of parent. Child should be not be
 *                NULL. If strlen(child) == 0, then true will be returned.
 * @param *parent: child is compared to parent. Parent should not be NULL.
 * @returns: If child is at any stage different to parent, then false is
 *           returned, otherwise true.
 */
extern bool cs_isprefix(const char * /*child*/, const char * /*parent*/);

#endif /* LIBCASSAVA_STRING_H */
/* vim: set cin ts=4 sw=4 et: */
