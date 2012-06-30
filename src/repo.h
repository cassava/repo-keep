/*
 * repo.h
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

#ifndef REPO_H
#define REPO_H

#include <stdbool.h>

/* get rid of assert or debug_* statements */
#define NDEBUG

#define REPO_VERSION    "2.0.1"
#define REPO_DATE       "30. June 2012"
#define REPO_VERSION_STRING "repo " REPO_VERSION "  (" REPO_DATE ")"

#define OK            0
#define ERR_MINOR     1
#define ERR_DEFAULT   2
#define ERR_SYSTEM    4
#define ERR_UNDEF     8

#define ARG_BUFFER      1024
#define CONFIG_PATH     "~/.repo.conf"
#define CONFIG_FAIL     0
#define CONFIG_LEN      2

#define SYSTEM_REPO_REMOVE "/usr/bin/repo-remove"
#define SYSTEM_REPO_ADD    "/usr/bin/repo-add"

/* use PKG_EXT only! */
#define PKG_STRICT_EXT  "-[0-9][a-z0-9._]*-[0-9]+-(any|i686|x86_64).pkg.tar.(gz|bz2|xz)$"
#define PKG_LENIENT_EXT "-[0-9].*-[0-9]+-(any|i686|x86_64).pkg.tar.(gz|bz2|xz)$"
#define PKG_EXT         PKG_LENIENT_EXT
#define PKG_NAME        "[a-zA-Z][a-zA-Z0-9_-]*"

/* Repo can execute a single command, which is one of the following. */
typedef enum action_command {
    action_add,             // add one or more packages
    action_remove,          // remove one or more packages
    action_update,          // automatically scan and add changed packages (by mod. date)
    action_sync,            // print out a list of outdated (according to AUR) packages
    action_list,            // list packages that are currently registered in the db
    action_nop              // no operation
} Action;

typedef struct arguments {
    bool soft;              // don't delete files
    bool noconfirm;         // don't ask before doing something
    bool verbose;           // be loud and verbose
    char *config;           // configuration file where next two values are stored
    char *db_name;          // config::database name
    char *db_dir;           // config::path to db location (with packages)
    char *db_path;          // db_name and db_path together
    Action command;         // command to execute (one of: sync, update, add, remove, list)
    char *argv[ARG_BUFFER]; // holds pointers to package arguments
    int argc;
} Arguments;

#endif // REPO_H

/* vim: set cin ts=4 sw=4 et: */
