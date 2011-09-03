/*
 * common.c 
 *
 * Copyright (c) 2010–2011 Ben Morgan <neembi@googlemail.com>
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

#include "common.h"
#include <stdio.h>
#include <string.h>

int file_exists(const char *file)
{
    FILE *in = fopen(file, "r");
    if (in == NULL)
        return 0;
    fclose(in);
    return 1;
}

/* concat: concatenates two strings.
 * Note: remember to free the memory when done.
 */
char *concat(const char *f1, const char *f2)
{
    int len;
    char *new;

    len = strlen(f1);
    new = (char *) malloc((len + strlen(f2) + 1) * sizeof (char));
    if (new != NULL) {
        strcpy(new, f1);
        strcpy(new+len, f2);
    }
    return new;
}

void repo_check(struct arguments *arguments)
{
    puts("repo_check()");
}