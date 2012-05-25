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

#define _VOID_CAST (void) (0)

#define debug(...) _VOID_CAST
#define debug_puts(str) _VOID_CAST
#define debug_printf(...) _VOID_CAST

#else

#include <stdio.h>

#define _LEVEL1(str) #str
#define _LEVEL0(str) _LEVEL1(str)
#define _DEBUG_FILENO __FILE__ " (" _LEVEL0(__LINE__) "): "

#define debug(...) __VA_ARGS__
#define debug_puts(str) puts(_DEBUG_FILENO str)
#define debug_printf(...) fprintf(stderr, _DEBUG_FILENO __VA_ARGS__)

#endif /* NDEBUG */

#endif /* DEBUG_H */
