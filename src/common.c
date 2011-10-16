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
#include <stdlib.h>


int file_readable(const char *file)
{
    FILE *in = fopen(file, "r");
    if (in == NULL)
        return 0;
    fclose(in);
    return 1;
}

int confirm(const char *question, int def)
{
    char c;
    
    printf("%s [%s] ", question, def ? "Y/n" : "y/N");

    c = getchar();
    if (def)
        switch (c) {
        case 'n':
        case 'N':
        case 'q':
        case 'Q':
            return 0;
        default:
            return 1;
        }
    else
        switch (c) {
        case 'y':
        case 'Y':
            return 1;
        default:
            return 0;
        }
}


void repo_check(struct arguments *arg)
{
    if (!file_readable(arg->db_path)) {
        fprintf(stderr, "Error: cannot open database '%s'\n", arg->db_path);
        exit (ERR_DB);
    } else if (arg->verbose) {
        printf("Found database '%s' at '%s'\n", arg->db_name, arg->db_dir);
    }
}
