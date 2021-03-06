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

#include "libcassava/debug.h"
#include "libcassava/list.h"
#include "libcassava/list_str.h"
#include "libcassava/string.h"
#include "libcassava/util.h"
#include "libcassava/system.h"

#ifdef NDEBUG
#define STRINGIFY_LEVEL1_(str) #str
#define STRINGIFY_LEVEL0_(str) STRINGIFY_LEVEL1_(str)
#define DEBUG_FILENO_ __FILE__ " (" STRINGIFY_LEVEL0_(__LINE__) "): "
#endif

static int remove_files(NodeStr *head, bool noconfirm);
static int add_package(const char *pkg_name, Arguments *arg);
static int exec_system(const char *command, bool verbose);
static char *pkg_name(const char *input);
static bool repo_check(Arguments *arg);
static bool file_readable(const char *file);
static bool confirm(const char *question, int def, bool noconfirm);

/* ------------------------------------------------------------------------- */

int repo_list(Arguments *arg)
{
    debug_puts("repo_list()");

    /* check prerequisites */
    if (!repo_check(arg))
        return ERR_SYSTEM;

    NodeStr *head;
    int retval = OK;
    int count = get_filenames_filter_regex(arg->db_dir, &head, ".*" PKG_EXT);
    if (count < 0) {
        fprintf(stderr, "Error: failed to retrieve files.\n");
        retval |= ERR_SYSTEM;
    } else if (count > 0) {
        char **array;
        size_t len = list_to_array(head, (void ***)&array);
        cs_qsort(array, len);

        for (size_t i = 0; i < len; i++)
            array[i] = pkg_name(array[i]);
        print_columns(array, len);

        // Cleanup:
        for (size_t i = 0; i < len; i++)
            free(array[i]);
        free(array);
    }

    list_free_all(&head);
    return ERR_UNDEF;
}


int repo_add(Arguments *arg)
{
    debug_puts("repo_add()");

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
    debug_puts("repo_remove()");

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
        count = get_filenames_filter_regex(arg->db_dir, &head, regex);
        free(argstr);
        free(regex);

        if (count == 0) {
            puts("No packages (files) found; nothing to remove.");
            list_free_all(&head);
            return retval;
        }

        remove_files(head, arg->noconfirm);
        list_free_all(&head);
    }

    /* remove entry from database */
    argstr = cs_strjoin(arg->argv, arg->argc, " ", 0);
    cmd = cs_strvcat(SYSTEM_REPO_REMOVE, " ", arg->db_path, " ", argstr, NULL);
    retval |= exec_system(cmd, arg->verbose);

    free(cmd);
    free(argstr);

    return retval;
}



int repo_update(Arguments *arg)
{
    debug_puts("repo_update()");

    time_t db_time;
    int retval = OK;
    NodeStr *head;

    /* check prerequisites */
    if (!repo_check(arg))
        return ERR_SYSTEM;

    /* get age of database */
    struct stat statbuf;
    if (stat(arg->db_path, &statbuf) == -1) {
        char *errmsg = cs_strvcat("Error: ", DEBUG_FILENO_, "stat '", arg->db_path, "'", NULL);
        perror(errmsg);
        free(errmsg);
        goto error;
    }
    db_time = statbuf.st_mtime;

    /* get all files younger than db_time */
    struct filter_time_args args = { db_time, 1 };
    retval = get_filenames_filter(".", &head, filter_mtime, (void *)&args);
    if (retval == -1)
        goto error;

    /* filter all files out that are not packages */
    char *pkgregex = cs_strvcat("^", PKG_NAME PKG_EXT, NULL);
    retval = list_filter_regex(&head, pkgregex);
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
                list_push(&short_head, name);
                add_package(name, arg);
            } else {
                free(name);
            }
        }
    }

    /* free list and return */
    list_free_all(&short_head);
    list_free_all(&head);
    return OK;

error:
    fprintf(stderr, "Fatal Error: " DEBUG_FILENO_ "cannot continue, exiting.\n");
    return ERR_UNDEF;
}


int repo_sync(Arguments *arg)
{
    debug_puts("repo_sync()");

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
    debug_puts("repo_check()");

    if (!file_readable(arg->db_path)) {
        fprintf(stderr, "Error: cannot open database '%s'\n", arg->db_path);
        return false;
    }
    return true;
}


/*
 * add_package: add a single package to the database.
 *
 * \warning we are assuming that we are in the directory arg->db_dir.
 */
static int add_package(const char *pkg_name, Arguments *arg)
{
    debug_printf("add_package(%s)\n", pkg_name);

    int retval = OK;
    NodeStr *head;

    char *regex = cs_strvcat("^(", pkg_name, ")", PKG_EXT, NULL);
    int count = get_filenames_filter_regex(".", &head, regex);
    free(regex);

    /* test age of file */
    if (count > 0) {
        char *cmd, *filename;
        time_t filetime = 0;

        printf("Found %d files for: %s\n", count, pkg_name);

        /* get youngest file */
        filename = NULL;
        for (NodeStr *iter = head; iter != NULL; iter = iter->next) {
            struct stat statbuf;

            if (stat(iter->data, &statbuf) == -1) {
                char *errmsg = cs_strvcat("Error: ", DEBUG_FILENO_, "stat '", iter->data, "'", NULL);
                perror(errmsg);
                free(errmsg);
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
            remove_files(oldest, arg->noconfirm);
            list_free_nodes(&oldest);
        }

        cmd = cs_strvcat(SYSTEM_REPO_ADD, " ", arg->db_path, " ", filename, NULL);
        retval |= exec_system(cmd, arg->verbose);
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
static int remove_files(NodeStr *head, bool noconfirm)
{
    debug_puts("remove_files()");

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
    if (confirm(mesg, 1, noconfirm)) {
        for (iter = head; iter != NULL; iter = iter->next) {
            printf("Removing file: %s\n", iter->data);
            if (remove(iter->data) != 0) {
                char *errmsg = cs_strvcat("Error: ", DEBUG_FILENO_, "remove '", iter->data, "'", NULL);
                perror(errmsg);
                free(errmsg);
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
    debug_puts("pkg_name()");

    const char *regex = "^(/.*/)?(" PKG_NAME ")" PKG_EXT;
    char errbuf[BUFSIZ];      /* for holding error messages by regex.h */
    int errcode;

    /* compile regex */
    regex_t preg;
    errcode = regcomp(&preg, regex, REG_EXTENDED);
    if (errcode != 0) {
        regerror(errcode, &preg, errbuf, sizeof errbuf);
        fprintf(stderr, "Error: " DEBUG_FILENO_ "regcomp: %s\n", errbuf);
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
        fprintf(stderr, "Error: " DEBUG_FILENO_ "regexec: %s\n", errbuf);
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
    debug_printf("file_readble(%s)\n", file);
    FILE *in = fopen(file, "r");
    if (in == NULL)
        return false;
    fclose(in);
    return true;
}


/*
 * confirm: ask the user a question and get an answer.
 * If the parameter noconfirm is false, the question is actually asked, and on
 * stderr. Otherwise, the default is accepted. (This is useful for documenting
 * what the system is doing if you used an option such as --noconfirm.)
 */
static bool confirm(const char *question, int def, bool noconfirm)
{
    debug_printf("confirm(%s)\n", question);
    char c = ' ';

    if (noconfirm) {
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
static int exec_system(const char *command, bool verbose)
{
    debug_printf("exec_system(%s)\n", command);
    int retval;

    if (verbose) printf("Running: %s\n", command);
    retval = system(command) == 0 ? OK : ERR_SYSTEM;
    return retval;
}

/* vim: set cin ts=4 sw=4 et: */
