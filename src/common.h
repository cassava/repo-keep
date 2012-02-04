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

#define REPO_VERSION    "1.9.2"
#define REPO_DATE       "4. February 2012"
#define REPO_VERSION_STRING "repo " REPO_VERSION "  (" REPO_DATE ")"

#define ERR_DB        1
#define ERR_REPO      2
#define ERR_PKG       4
#define ERR_CONFIG    8
#define ERR_CMD       64
#define ERR_UNDEF     128

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
#define PKG_NAME        "[a-zA-Z][a-zA-Z0-9-]*"


typedef struct arguments {
    int soft;               // don't delete files
    int natural;            // don't compare file creation times
    int quiet;              // don't ask before doing something
    char *config;           // configuration file where next two values are stored
    char *db_name;          // config::database name
    char *db_dir;           // config::path to db location (with packages)
    char *db_path;          // db_name and db_path together
    char *command;          // command to execute (one of: sync, update, add, remove)
    char *argv[ARG_BUFFER]; // holds pointers to package arguments
    int argc;
} Arguments;

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
