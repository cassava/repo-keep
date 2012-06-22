/*
 * libcassava/system.h
 * vim: set cin ts=4 sw=4 et:
 *
 * Copyright (c) 2012 Ben Morgan <neembi@googlemail.com>
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

/**
 * \file
 * Functions that are pertinent to the system and the filesystem.
 *
 * \author Ben Morgan
 * \date 28. May 2012
 */

#ifndef LIBCASSAVA_SYSTEM_H
#define LIBCASSAVA_SYSTEM_H

#include <assert.h>
#include <regex.h>
#include <stdbool.h>

#include "list.h"
#include "list_str.h"

/**
 * Get the number of columns in the current terminal.
 * This also works correctly if the terminal has been resized.
 *
 * If you call this, and stdout is a file for example, then it will return 0,
 * because there is no reasonable input. In that case you ought to think up a
 * reasonable default, 80 for example.
 *
 * \return Number of columns in current terminal.
 */
extern unsigned short get_terminal_columns();

extern int read_directory(const char *path, NodeStr **head, bool full_pathnames);

inline int get_filenames(const char *path, NodeStr **head)
{
    return read_directory(path, head, false);
}

inline int get_filenames_filter(const char *path,
                                NodeStr **head,
                                bool (*filter)(void *path, void *arguments),
                                void *arguments)
{
    assert(filter != NULL);

    int count = get_filenames(path, head);
    if (count > 0)
        count = list_filter(head, filter, arguments);
    return count;
}

inline int get_filepaths(const char *path, NodeStr **head)
{
    return read_directory(path, head, true);
}

inline int get_filepaths_filter(const char *path,
                                NodeStr **head,
                                bool (*filter)(void *path, void *arguments),
                                void *arguments)
{
    assert(filter != NULL);

    int count = get_filepaths(path, head);
    if (count > 0)
        count = list_filter(head, filter, arguments);
    return count;
}

extern int read_directory_filter_regex(const char *path, NodeStr **head, const char *regex, bool full_pathnames);

inline int get_filepaths_filter_regex(const char *path, NodeStr **head, const char *regex)
{
    return read_directory_filter_regex(path, head, regex, true);
}

inline int get_filenames_filter_regex(const char *path, NodeStr **head, const char *regex)
{
    return read_directory_filter_regex(path, head, regex, false);
}

extern bool filter_isreg(void *filepath, void *);

extern bool filter_isdir(void *filepath, void *);

struct filter_time_args {
    time_t time;
    int comparison;
};

extern bool filter_mtime(void *filepath, void *arguments);

#endif /* LIBCASSAVA_SYSTEM_H */
