/*
 * actions.c
 * Includes the code for all the actions:
 *   add, remove, list, update, sync.
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

#include "repo.h"
#include "actions.h"

#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <libgen.h>
#include <regex.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "cassava/debug.h"
#include "cassava/list.h"
#include "cassava/list_str.h"
#include "cassava/string.h"
#include "cassava/util.h"

static int remove_files(NodeStr *head, int quiet);
static int add_package(const char *pkg_name, Arguments *arg);
static int exec_system(const char *command);
static char *pkg_name(const char *input);
static bool repo_check(Arguments *arg);
static bool file_readable(const char *file);
static bool confirm(const char *question, int def, int quiet);

/* ------------------------------------------------------------------------- */

int repo_list(Arguments *arg)
{
    NodeStr *head;
    int count;
    int retval = OK;

    /* check prerequisites */
    if (!repo_check(arg))
        return ERR_SYSTEM;

    count = get_regex_files(".*" PKG_EXT, arg->db_dir, &head);
    if (count < 0) {
        fprintf(stderr, "Error: failed to retrieve files.\n");
        retval |= ERR_SYSTEM;
    } else if (count > 0) {
        for (NodeStr *iter = head; iter != NULL; iter = iter->next) {
            char *name = pkg_name(iter->data);
            assert(name != NULL);
            printf("  %s\n", name);
            free(name);
        }
    }

    list_free_all(&head);
    return ERR_UNDEF;
}


int repo_add(Arguments *arg)
{
    int retval = OK;

    /* check prerequisites */
    if (!repo_check(arg))
        return ERR_SYSTEM;

    /* iterate through packages to add */
    for (int i = 0; i < arg->argc; i++)
        retval |= add_package(arg->argv[i], arg);

    return retval;
}


int repo_remove(Arguments *arg)
{
    char *cmd, *argstr;
    int retval = OK;

    /* check prerequisites */
    if (!repo_check(arg))
        return ERR_SYSTEM;

    /* if files should be removed, remove files */
    if (!arg->soft) {
        char *regex;
        NodeStr *head;  /* head of a linked list of filenames */
        int count;

        argstr = cs_strjoin(arg->argv, arg->argc, "|", 0);
        regex = cs_strvcat("^(", argstr, ")", PKG_EXT, NULL);
        count = get_regex_files(regex, arg->db_dir, &head);
        free(argstr);
        free(regex);

        if (count == 0) {
            puts("No packages (files) found; nothing to remove.");
            list_free_all(&head);
            return retval;
        }

        remove_files(head, arg->quiet);
        list_free_all(&head);
    }

    /* remove entry from database */
    argstr = cs_strjoin(arg->argv, arg->argc, " ", 0);
    cmd = cs_strvcat(SYSTEM_REPO_REMOVE, " ", arg->db_path, " ", argstr, NULL);
    retval |= exec_system(cmd);

    free(cmd);
    free(argstr);

    return retval;
}


int repo_update(Arguments *arg)
{
    time_t db_time;
    int retval = OK;
    NodeStr *head;

    /* check prerequisites */
    if (!repo_check(arg))
        return ERR_SYSTEM;

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
    char *pkgregex = cs_strvcat("^", arg->db_dir, PKG_NAME PKG_EXT, NULL);
    retval = list_filter_destroy(&head, pkgregex);
    free(pkgregex);
    if (retval == -1) {
        goto error;
    } else if (retval == 0) {
        printf("Database up-to-date: nothing to do.\n");
        return OK;
    }

    printf("Found %d packages younger than database:\n", retval);
    list_println(head, "    ");
    printf("\n");

    /* add packages */
    NodeStr *short_head = NULL;
    for (NodeStr *iter = head; iter != NULL; iter = iter->next) {
        char *name = pkg_name(iter->data);
        if (name != NULL) {
            /*
             * If name is not in $short_head, then it is added, and will be freed
             * later by list_free_all(&short_head). Otherwise it is not added, and
             * we need to delete it ourselves.
             */
            if (list_search(short_head, name) == NULL) {
                debug_printf("debug: pushing name %s\n", name);
                list_push(&short_head, name);
                add_package(name, arg);
            } else {
                debug_printf("debug: freeing name %s\n", name);
                free(name);
            }
        }
        debug(list_print(short_head, " "), printf("\n"));
    }

    /* free list and return */
    debug(list_print(short_head, " "), printf("\n"));
    debug(list_print(head, " "), printf("\n"));
    list_free_all(&short_head);
    list_free_all(&head);
    return OK;

error:
    fprintf(stderr, "Error (update.c): cannot continue, exiting.\n");
    return ERR_UNDEF;
}


int repo_sync(Arguments *arg)
{
    /* check prerequisites */
    if (!repo_check(arg))
        return ERR_SYSTEM;

    fprintf(stderr, "\n=== THIS FUNCTION HAS NOT BEEN IMPLEMENTED YET! ===\n");
    return ERR_UNDEF;
}

/* ------------------------------------------------------------------------- */

/*
 * repo_check: print an error message and quit in case there is anything
 * wrong with db_name and db_dir given in the configuration file.
 */
static bool repo_check(Arguments *arg)
{
    if (!file_readable(arg->db_path)) {
        fprintf(stderr, "Error: cannot open database '%s'\n", arg->db_path);
        return false;
    }
    return true;
}


/*
 * add_package: add a single package to the database.
 */
static int add_package(const char *pkg_name, Arguments *arg)
{
    int retval = OK;
    char *regex;
    NodeStr *head;
    int count;

    regex = cs_strvcat("^(", pkg_name, ")", PKG_EXT, NULL);
    count = get_regex_files(regex, arg->db_dir, &head);
    free(regex);

    /* test age of file */
    if (count > 0) {
        char *cmd, *filename;
        time_t filetime = 0; // TODO: check if this is right

        printf("Found %d files for: %s\n", count, pkg_name);

        /* get youngest file */
        filename = NULL;
        for (NodeStr *iter = head; iter != NULL; iter = iter->next) {
            struct stat statbuf;

            if (stat(iter->data, &statbuf) == -1) {
                perror("Error (stat)");
                retval |= ERR_MINOR;
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
            NodeStr *oldest = NULL;

            for (NodeStr *iter = head; iter != NULL; iter = iter->next) {
                if (iter->data == filename)
                    continue;

                list_push(&oldest, iter->data);
            }

            printf("Keeping: %s\n", filename);
            remove_files(oldest, arg->quiet);
            list_free_nodes(&oldest);
        }

        cmd = cs_strvcat(SYSTEM_REPO_ADD, " ", arg->db_path, " ", filename, NULL);
        retval |= exec_system(cmd);
        free(cmd);
    } else {
        fprintf(stderr, "Error: did not find any files to add.\n");
        retval |= ERR_DEFAULT;
    }

    list_free_all(&head);
    return retval;
}


/*
 * remove_files: confirm the removal of list of files, and remove them.
 */
static int remove_files(NodeStr *head, int quiet)
{
    NodeStr *names, *iter;
    char *args, *mesg;
    int retval = OK;

    /* transform list head into only filenames */
    names = NULL;
    for (iter = head; iter != NULL; iter = iter->next) {
        list_push(&names, basename(iter->data));
    }

    /* use names list to create a question */ 
    args = list_strjoin(names, "\n              ");
    mesg = cs_strvcat("Delete files: ", args, "?", NULL);
    free(args);
    list_free_nodes(&names);

    /* ask if user wants to delete all the files and do it */
    if (confirm(mesg, 1, quiet)) {
        for (iter = head; iter != NULL; iter = iter->next) {
            printf("Removing file: %s\n", iter->data);
            if (remove(iter->data) != 0) {
                perror("Error (remove)");
                retval |= ERR_MINOR;
            }
        }
    }

    return retval;
}

/*
 * pkg_name: get the name of a package from the entire path and name.
 * Returns: name of the package, or NULL if pkg_path does not match.
 * Warning: you must call free() on the result of this function.
 */
static char *pkg_name(const char *input)
{
    const char *regex = "^(/.*/)?(" PKG_NAME ")" PKG_EXT;
    char errbuf[MAX_ERROR_LINE_LENGTH];      /* for holding error messages by regex.h */
    int errcode;

    /* compile regex */
    regex_t preg;
    errcode = regcomp(&preg, regex, REG_EXTENDED);
    if (errcode != 0) {
        regerror(errcode, &preg, errbuf, sizeof errbuf);
        fprintf(stderr, "Error (regcomp): %s\n", errbuf);
        return NULL;
    }

    char *result = NULL;
    regmatch_t pmatch[3];
    errcode = regexec(&preg, input, 3, pmatch, 0);
    if (errcode == 0) {
        result = cs_substr(input, pmatch[2].rm_so, pmatch[2].rm_eo);
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
 * file_readable: return whether the file in question is readable or not.
 */
static bool file_readable(const char *file)
{
    FILE *in = fopen(file, "r");
    if (in == NULL)
        return false;
    fclose(in);
    return true;
}


/*
 * confirm: ask the user a question and get an answer.
 * If the parameter quiet is false, the question is actually asked, and on
 * stderr. Otherwise, the default is accepted. (This is useful for documenting
 * what the system is doing if you used an option such as --quiet.)
 */
static bool confirm(const char *question, int def, int quiet)
{
    char c = ' ';

    if (quiet) {
        printf("%s [%s] .\n", question, def ? "Y/n" : "y/N");
    } else {
        fprintf(stderr, "%s [%s] ", question, def ? "Y/n" : "y/N");
        c = getchar();
    }

    if (def)
        switch (c) {
        case 'n':
        case 'N':
        case 'q':
        case 'Q':
            return false;
        default:
            return true;
        }
    else
        switch (c) {
        case 'y':
        case 'Y':
            return true;
        default:
            return false;
        }
}


/*
 * exec_system: print the command, run it in the system, return status.
 *
 * @returns: OK or ERR_SYSTEM.
 */
static int exec_system(const char *command) {
    int retval;

    printf("Running: %s\n", command);
    retval = system(command) == 0 ? OK : ERR_SYSTEM;
    return retval;
}

/* vim: set cin ts=4 sw=4 et: */
