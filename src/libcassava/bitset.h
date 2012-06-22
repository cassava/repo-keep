/*
 * libcassava/bitset.h
 * vim: set cindent expandtab tabstop=4 shiftwidth=4 colorcolumn=81:
 *
 * Copyright (c) 2012 Ben Morgan <neembi@googlemail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * \file
 * Defines functions for manipulating a set of bits.
 *
 * These functions are derived from the bitset class in C++.
 * The function definitions are so short that they could even be inline, but at
 * the moment they just aren't. Look at the source if you want and incorporate
 * it that way into your program if you want them inline (or as macros).
 *
 * The implementation is simple; we have an array of unsigned ints and we use
 * bit operators to manipulate the individual bits in each of these unsigned
 * integers, plus a little arithmetic to calculate which int we need from the
 * array.
 *
 * <b>Example Usage:</b>
 * \code
 *     #include <cassava/bitset.h>  // for new_bitset, get_bit, set_bit
 *     #include <stdbool.h>         // for bool
 *     #include <stdlib.h>          // for size_t
 *     #include <stdio.h>           // for putchar
 *     #include <string.h>          // for strlen
 *  
 *     int main()
 *     {
 *         char *binstr = "010101010101";
 *         bitset_t *bs = new_bitset(strlen(binstr));
 *         size_t count = 0;
 *         while (*binstr != '\0')
 *             set_bit(bs, count++, *binstr++ == '1' ? true : false);
 *         size_t i = 0;
 *         while (i < count)
 *             putchar(get_bit(bs, i++) ? '1' : '0');
 *         putchar('\n');
 *         free(bs);
 *         return 0;
 *     }
 * \endcode
 *
 * \author Ben Morgan
 * \date 1. June 2012
 */

#ifndef LIBCASSAVA_BITSET_H
#define LIBCASSAVA_BITSET_H

#include <stdbool.h>
#include <stdlib.h>

typedef unsigned int bitset_t;

/**
 * Returns a newly allocated bitset, with space for at least \a size elements.
 *
 * How much more space there might be varies from machine to machine; on some
 * machines it might range from 0 to 63 for example, depending on the value of
 * \a size.
 *
 * \param size Number of bits which can be stored.
 * \return Pointer to newly allocated (with malloc) bitset.
 * \note The result of this function must be at some point freed.
 */
bitset_t *new_bitset(size_t size);

/**
 * Get a bit stored at position \a pos in the bitset \a array.
 *
 * Behaviour is undefined if \a pos is outside of bitset array bounds.
 */
bool get_bit(const bitset_t *array, size_t pos);

/**
 * Set a bit stored at position \a pos in the bitset \a array.
 *
 * Behaviour is undefined if \a pos is outside of bitset array bounds.
 */
void set_bit(bitset_t *array, size_t pos, bool value);

#endif /* LIBCASSAVA_BITSET_H */
