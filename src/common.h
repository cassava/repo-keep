/*
 * common.h
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

#ifndef COMMON_H
#define COMMON_H

#define REPO_VERSION    "2.0"
#define REPO_DATE       "4. September 2011"
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

#define SYSTEM_REPO_REMOVE "repo-remove"
#define SYSTEM_REPO_ADD    "repo-add"

#define PKG_STRICT_EXT  "-[a-z0-9._]+-[0-9]+-(any|i686|x86_64).pkg.tar.(gz|bz2|xz)"
#define PKG_LENIENT_EXT "-.+-[0-9]+-(any|i686|x86_64).pkg.tar.(gz|bz2|xz)"


struct arguments {
    int soft;               // don't delete files
    int natural;            // don't compare file creation times
    int quiet;              // don't print any unnecessary output
    int verbose;            // tell me more!
    char *config;           // configuration file where next two values are stored
    char *db_name;          // config::database name
    char *db_dir;           // config::path to db location (with packages)
    char *db_path;          // db_name and db_path together
    char *command;          // command to execute (one of: sync, update, add, remove)
    char *argv[ARG_BUFFER]; // holds pointers to package arguments
    int argc;
};

/* in common.c */
int file_readable(const char *);
//char *file_age_cmp(char *, char **, int);
void repo_check(struct arguments *);

/* in update.c */
void repo_update(struct arguments *);
void repo_add(struct arguments *);
void repo_remove(struct arguments *);

/* in sync.c */
void repo_sync(struct arguments *);

#endif // COMMON_H
