/*
 * debug.h
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
#ifndef DEBUG_H
#define DEBUG_H

#ifdef NDEBUG

#define VOID_POINTER_CAST_ (void) (0)

#define debug(...) VOID_POINTER_CAST_
#define debug_puts(str) VOID_POINTER_CAST_
#define debug_printf(...) VOID_POINTER_CAST_

#else

#include <stdio.h>

#define STRINGIFY_LEVEL1_(str) #str
#define STRINGIFY_LEVEL0_(str) STRINGIFY_LEVEL1_(str)
#define DEBUG_FILENO_ __FILE__ " (" STRINGIFY_LEVEL0_(__LINE__) "): "

#define debug(...) __VA_ARGS__
#define debug_puts(str) puts(DEBUG_FILENO_ str)
#define debug_printf(...) fprintf(stderr, DEBUG_FILENO_ __VA_ARGS__)

#endif /* NDEBUG */

#endif /* DEBUG_H */
