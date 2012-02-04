/*
 * common.h
 * 
 * Copyright (c) 2011-2012 Ben Morgan <neembi@googlemail.com>
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

#ifndef COMMON_H
#define COMMON_H

#include "repo.h"

/* in common.c */
extern int file_readable(const char *);
extern int confirm(const char *, int /*def*/, int /*quiet*/);
extern void repo_check(Arguments *);

/* in update.c */
extern void repo_update(Arguments *);
extern void repo_add(Arguments *);
extern void repo_remove(Arguments *);

/* in sync.c */
extern void repo_sync(Arguments *);

#endif // COMMON_H
