/*
 * bm_util.c 
 *
 * Copyright (c) 2011 Ben Morgan <neembi@googlemail.com>
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

#include "bm_util.h"
#include "bm_list.h"
#include "bm_string.h"
#include <regex.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>


/*
 * get_regex_files: get all files in dir that match regex.
 * Returns: number of files found.
 * Warning: make sure to call list_free_all() on the head Node when done.
 */
int get_regex_files(const char *regex, const char *dir, Node **head)
{
    char errbuf[MAX_OUT_LINE];      /* for holding error messages by regex.h */
    struct dirent *entry;
    DIR *dp;
    int errcode;
    int count;

    /* initialize head and count */
    *head = NULL;
    count = 0;

    /* open db dir */
    dp = opendir(dir);
    if (dp == NULL) {
        perror("Error (opendir)");
        return -1;
    }
    
    /* compile regex */
    regex_t preg;
    errcode = regcomp(&preg, regex, REG_EXTENDED);
    if (errcode != 0) {
        regerror(errcode, &preg, errbuf, sizeof errbuf);
        fprintf(stderr, "Error (regcomp): %s\n", errbuf);
        return -1;
    }

    Node *current = NULL;
    int complete = (*(dir+strlen(dir)-1) == '/');
    while ((entry = readdir(dp)) != NULL) {
        /* make sure type is not dir, then do a regex on it */
        struct stat fstat;
        char *path = bm_strvcat(dir, complete ? "" : "/", entry->d_name, NULL);
        if (stat(path, &fstat) != 0) {
            perror("stat");
            free(path);
            return -1;
        }
        if (! S_ISREG(fstat.st_mode))
            goto free_path_continue;

        errcode = regexec(&preg, entry->d_name, 0, NULL, 0);
        if (errcode == 0) {
            Node *new = list_node();
            new->data = path;
            if (current == NULL) {
                *head = current = new;
            } else {
                current->next = new;
                current = new;
            }
            count++;
        } else if (errcode == REG_NOMATCH) {
            goto free_path_continue;
        } else {
            free(path);
            regerror(errcode, &preg, errbuf, sizeof errbuf);
            fprintf(stderr, "Error (regexec): %s\n", errbuf);
            return -1;
        }
        continue;

    free_path_continue:
        free(path);
    }

    closedir(dp);
    regfree(&preg);
    if (current != NULL)
        current->next = NULL;
    return count;
}


/*
 * list_files: print a listing of all the files in linked list.
 */
void list_files(Node *head)
{
    Node *node;

    node = head;
    while (node != NULL) {
        puts(node->data);
        node = node->next;
    }
}


/*
 * list_strjoin: join all the list elements together.
 * Returns the joined string. Don't forget to free it when done.
 */
char *list_strjoin(Node *head, const char *delim)
{
    char *str, *t;
    const char *s;
    size_t len = 1;
    int m, n;
    Node *node;

    n = list_length(head);
    if (n == 0)
        return NULL;

    m = strlen(delim);
    len += (n-1) * m;
    for (node = head; node != NULL; node = node->next)
        len += strlen(node->data);

    t = str = malloc(len * sizeof (char));
    for (node = head; node != NULL; node = node->next, n--) {
        s = node->data;
        while ((*t = *s) != '\0')
            t++, s++;
        if (n > 1) {
            s = delim;
            while ((*t = *s) != '\0')
                t++, s++;
        }
    }
    return str;
}
