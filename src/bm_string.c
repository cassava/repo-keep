/*
 * bm_string.c
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

#include "bm_string.h"

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


char *bm_strcat(const char *f1, const char *f2)
{
    size_t len;
    char *str;

    len = strlen(f1);
    str = malloc((len + strlen(f2) + 1) * sizeof (char));
    if (str != NULL) {
        strcpy(str, f1);
        strcpy(str+len, f2);
    }
    return str;
}

char *bm_strjoin(char **arr, int n, char *delim, int extra)
{
    char *str, *s, *t;
    size_t len = extra + 1;
    int i, m;

    m = strlen(delim);
    if (m == 0)
        return bm_stracat(NULL, arr, n, extra);
    len += (n-1) * m;
    for (i = 0; i < n; i++)
        len += strlen(arr[i]);
    t = str = malloc(len * sizeof (char));
    for (i = 0; i < n; i++) {
        s = arr[i];
        while ((*t = *s) != '\0')
            (t++, s++);
        s = delim;
        while ((*t = *s) != '\0')
            (t++, s++);
    }
    *(t-m) = '\0';
    return str;
}

char *bm_stracat(const char *f, char **arr, int n, int extra)
{
    char *str, *s, *t;
    size_t len = extra + 1;
    int i;

    if (f != NULL)
        len += strlen(f);
    for (i = 0; i < n; i++) {
        len += strlen(arr[i]);
    }
    t = str = malloc(len * sizeof (char));
    if (f != NULL)
        while ((*t = *f) != '\0')
            (t++, f++);
    for (i = 0; i < n; i++) {
        s = arr[i];
        while ((*t = *s) != '\0')
            (t++, s++);
    }
    return str;
}

char *bm_strvcat(const char *f, ...)
{
    va_list ap;
    char *str, *s, *t;
    size_t len = 1;

    // determine length of final string
    va_start(ap, f); s=f;
    while (s != NULL) {
        len += strlen(s);
        s = va_arg(ap, char *);
    }
    va_end(ap);
    
    // create new string
    str = t = malloc(len * sizeof (char));
    if (str != NULL) {
        va_start(ap, f);
        s=f; // we start with f
        while (s != NULL) {
            strcpy(t, s);
            t += strlen(s);
            s = va_arg(ap, char *);
        }
        va_end(ap);
    }

    return str;
}
