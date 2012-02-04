/*
 * update.c
 * Includes the code for the commands 'add', 'remove' and 'update'.
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

#include "repo.h"
#include "common.h"
#include "bm_string.h"
#include "bm_util.h"
#include "bm_list.h"
#include <dirent.h>
#include <regex.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>


/*
 * pkg_name: get the name of a package from the entire path and name.
 * Returns: name of the package, or NULL if pkg_path does not match.
 * Warning: you must call free() on the result of this function.
 */
char *pkg_name(const char *input)
{
    const char *regex = "^(/.*/)?(" PKG_NAME ")" PKG_EXT;
    char errbuf[MAX_OUT_LINE];      /* for holding error messages by regex.h */
    int errcode;

    /* compile regex */
    regex_t preg;
    errcode = regcomp(&preg, regex, REG_EXTENDED);
    if (errcode != 0) {
        regerror(errcode, &preg, errbuf, sizeof errbuf);
        fprintf(stderr, "Error (regcomp): %s\n", errbuf);
        return NULL;
    }

    // FIXME: This does not work yet!
    char *result = NULL;
    regmatch_t pmatch[3];
    errcode = regexec(&preg, input, 3, pmatch, 0);
    if (errcode == 0) {
        result = bm_substr(input, pmatch[2].rm_so, pmatch[2].rm_eo);
    } else if (errcode == REG_NOMATCH) {
        goto end;
    } else {
        regerror(errcode, &preg, errbuf, sizeof errbuf);
        fprintf(stderr, "Error (regexec): %s\n", errbuf);
        goto end;
    }

end:
    regfree(&preg);
    return result;
}

/*
 * remove_files: confirm the removal of list of files, and remove them.
 */
void remove_files(Node *head, int quiet)
{
    Node *names, *iter;
    char *args, *mesg;

    /* transform list head into only filenames */
    names = NULL;
    for (iter = head; iter != NULL; iter = iter->next) {
        Node *temp = list_node();
        temp->data = basename(iter->data);
        list_insert(&names, temp);
    }

    /* use names list to create a question */ 
    args = list_strjoin(names, "\n              ");
    mesg = bm_strvcat("Delete files: ", args, "?", NULL);
    free(args);
    list_free_nodes(names);

    /* ask if user wants to delete all the files and do it */
    if (confirm(mesg, 1, quiet)) {
        for (iter = head; iter != NULL; iter = iter->next) {
            printf("Removing file: %s\n", iter->data);
            if (remove(iter->data) != 0)
                perror("Error (remove)");
        }
    }
}


/*
 * add_package: add a single package to the database.
 * If definitely not successful, returns 0, otherwise 1.
 */
int add_package(const char *pkg_name, Arguments *arg)
{
    char *regex;
    Node *head;
    int count;

    regex = bm_strvcat("^(", pkg_name, ")", PKG_EXT, NULL);
    count = get_regex_files(regex, arg->db_dir, &head);
    free(regex);

    /* test age of file */
    if (count > 0) {
        char *cmd, *filename;
        time_t filetime = 0; // TODO: check if this is right

        printf("Found %d files for: %s\n", count, pkg_name);

        /* get youngest file */
        filename = NULL;
        for (Node *iter = head; iter != NULL; iter = iter->next) {
            struct stat statbuf;

            if (stat(iter->data, &statbuf) == -1) {
                perror("Error (stat)");
                continue;
            }

            if (statbuf.st_mtime > filetime) {
                filename = iter->data;
                filetime = statbuf.st_mtime;
            }
        }

        /* delete files if we're not soft */
        if (count > 1 && !arg->soft) {
            /* put oldest files into a list */
            Node *oldest = NULL;

            for (Node *iter = head; iter != NULL; iter = iter->next) {
                if (iter->data == filename)
                    continue;
                
                Node *temp = list_node();
                temp->data = iter->data;
                list_insert(&oldest, temp);
            } 

            printf("Keeping: %s\n", filename);
            remove_files(oldest, arg->quiet);
            list_free_nodes(oldest);
        }

        cmd = bm_strvcat(SYSTEM_REPO_ADD, " ", arg->db_path, " ", filename, NULL);
        printf("Running: %s\n", cmd);
        system(cmd);
        free(cmd);
    } else {
        fprintf(stderr, "Error: did not find any files to add.\n");
        return 0;
    }

    list_free_all(head);
    return 1;
}


/*
 * repo_add: add the packages given in *arg from the database,
 * remove old packages from the filesystem (if !arg->soft).
 */
void repo_add(Arguments *arg)
{
    repo_check(arg);
    
    /* iterate through packages to add */
    for (int i = 0; i < arg->argc; i++)
        add_package(arg->argv[i], arg);
}


/*
 * repo_remove: remove the packages given in *arg from the database and maybe
 * also from the filesystem (if !arg->soft).
 */
void repo_remove(Arguments *arg)
{
    char *cmd, *argstr;

    repo_check(arg);

    /* if files should be removed, remove files */
    if (!arg->soft) {
        char *regex;
        Node *head;  /* head of a linked list of filenames */
        int count;

        argstr = bm_strjoin(arg->argv, arg->argc, "|", 0);
        regex = bm_strvcat("^(", argstr, ")", PKG_EXT, NULL);
        count = get_regex_files(regex, arg->db_dir, &head);
        free(argstr);
        free(regex);

        if (count == 0) {
            puts("No packages (files) found; nothing to remove.");
            list_free_all(head);
            return;
        }

        remove_files(head, arg->quiet);
        list_free_all(head);
    }

    /* remove entry from database */
    argstr = bm_strjoin(arg->argv, arg->argc, " ", 0);
    cmd = bm_strvcat(SYSTEM_REPO_REMOVE, " ", arg->db_path, " ", argstr, NULL);
    printf("Running: %s\n", cmd);
    system(cmd);

    free(cmd);
    free(argstr);
}


/*
 * repo_update: scan the directory where the database resides,
 * and add all packages that are newer than the database.
 */
void repo_update(Arguments *arg)
{
    time_t db_time;
    int retval;
    Node *head;

    /* check prerequisites */
    repo_check(arg);

    /* get age of database */
    struct stat statbuf;
    if (stat(arg->db_path, &statbuf) == -1) {
        perror("Error (stat)");
        goto error;
    }
    db_time = statbuf.st_mtime;

    /* get all files younger than db_time */
    if ((retval = get_younger_files(db_time, arg->db_dir, &head)) == -1)
        goto error;

    /* filter all files out that are not packages */
    char *pkgregex = bm_strvcat("^", arg->db_dir, PKG_NAME PKG_EXT, NULL);
    retval = list_filter_destroy(pkgregex, &head);
    free(pkgregex);
    if (retval == -1) {
        goto error;
    } else if (retval == 0) {
        printf("Database up-to-date: nothing to do.\n");
        return;
    } 

    printf("Found %d packages younger than database:\n", retval);
    list_files(head, "    ");
    printf("\n");

    /* add packages */
    Node *short_head = NULL;
    for (Node *iter = head; iter != NULL; iter = iter->next) {
        char *name = pkg_name(iter->data);
        if (name != NULL && list_search(name, short_head) == NULL) {
            Node *temp = list_node();
            temp->data = name;
            list_insert(&head, temp);

            add_package(name, arg);
        }
    }

    /* free list and return */
    list_free_all(short_head);
    list_free_all(head);
    return;

error:
    fprintf(stderr, "Fatal error: cannot continue, exiting.");
    exit(1);
}
