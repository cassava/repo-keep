/*
 * config_bm.c
 * For parsing the configuration file.
 * 
 * Spaces and comments starting with # are ignored.  Comments must be by
 * themselves on a line.  Keys are separated from values by the '=' character,
 * and may contain any character except the '=' character.  The value may
 * contain any character, but is ended by a newline.
 */

/* 
 * Copyright (c) 2010–2011 Ben Morgan <neembi@googlemail.com>
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

#include "config_bm.h"

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

static struct config_map *binsearch(char *, struct config_map *, int);
static char *strtrim(char *);

int config_parse(const char *path, struct config_map *tab, int fail)
{
    FILE *in = fopen(path, "r");
 
    if (in != NULL) {
        char line[LINE_MAX+1];
        char *value, *c;
        int tabsize;
        struct config_map *map;

        for (tabsize = 1; tab[tabsize].key != NULL; tabsize++)
            ;
 
        while (fgets(line, sizeof line, in) != NULL) {
            // find position with first non whitespace
            size_t i = strspn(line, " \t\n\v");

            // skip comments
            if (line[i] == '\0' || line[i] == '#')
                continue;
 
            // Process non-comment line
            // split line into two, by overwriting the first '=' to '\0'
            c = value = strchr(&line[i], '=');
            if (value == NULL) {
                fprintf(stderr, "error: invalid line in configuration file '%s':\n"
                                "       '%s'", path, line);
                if (fail)
                    return 1;
                else
                    continue;
            }
            *value++ = '\0';
            // trim the whitespace from the end of the key
            while (isspace(*--c))
                ;
            *++c = '\0';
            // trim the whitespace from the value
            value = strtrim(value);
            
            // get correct map for the particular key (takes O(log n) time)
            map = binsearch(line, tab, tabsize);
            if (map == NULL) {
                fprintf(stderr, "error: invalid key in configuration file '%s':\n"
                                "       '%s'\n", path, line);
                if (fail)
                    return 2;
                else
                    continue;
            }
            
            // am not checking if malloc returns NULL because in this case moot.
            map->value = (char *) malloc((strlen(value)+1) * sizeof (char));
            strcpy(map->value, value);
        }
    } else {
        // some error happened while opening the file
        fprintf(stderr, "error: could not open configuration file '%s'\n", path);
        return -1;
    }
 
    return 0;
}


/* binsearch: find word in tab[0]...tab[n-1].
 * We are assuming that tab is sorted with
 *   tab[i] < tab[i+1] for all i
 */
static struct config_map *binsearch(char *word, struct config_map *tab, int n)
{
    int cond;
    struct config_map *low = &tab[0];
    struct config_map *high = &tab[n];
    struct config_map *mid;

    while (low < high) {
        mid = low + (high-low) / 2;
        if ((cond = strcmp(word, mid->key)) < 0)
            high = mid;
        else if (cond > 0)
            low = mid + 1;
        else
            return mid;
    }
    return NULL;
}

/* strtrim: trim the whitespace from a _modifiable_ string */
static char *strtrim(char *word)
{
    char *end;

    while (isspace(*word))
        word++;
    end = word;
    if (*word != '\0') {
        while (*end != '\0')
            end++;
        while (isspace(*--end))
            ;
        *++end = '\0';
    }
    return word;
}

