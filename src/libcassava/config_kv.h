/*
 * libcassava/config_kv.h
 * For parsing a simple key-value configuration file.
 *
 * Spaces and comments starting with # are ignored.  Comments must be by
 * themselves on a line.  Keys are separated from values by the '=' character,
 * and may contain any character except the '=' character.  The value may
 * contain any character, but is ended by a newline.
 */

/*
 * Copyright (c) 2011-2012 Ben Morgan <neembi@googlemail.com>
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

#ifndef LIBCASSAVA_CONFIG_KV_H
#define LIBCASSAVA_CONFIG_KV_H

#include <stdbool.h>

#define CONFIG_KV_NOERR  0
#define CONFIG_KV_EFILE -1
#define CONFIG_KV_ELINE  1
#define CONFIG_KV_EKEY   2

#ifndef LINE_MAX
#define LINE_MAX 1024
#endif

struct config_map {
    char *key;
    char *value;
};

/*
 * config_parse: parse a simple key-value configuration file.
 * Arguments:
 *   path      = path to the file to parse
 *   config_map= map to fill with values (based on keys)
 *   fail      = whether to fail on errors in the config file
 * @return:  CONFIG_KV_NOERR  = no problems detected
 *           CONFIG_KV_EFATAL = file described by path could not be opened
 *                              OR memory could not be allocated.
 *           CONFIG_KV_ELINE  = (if fail != 0) on invalid line encountered
 *           CONFIG_KV_EKEY   = (if fail != 0) on invalid key encountered
 * Note: You must check tab yourself to see if any values have
 * changed. If you have values set to NULL, they may still be
 * NULL if the key was not found in the config file. All to say,
 * a config file that exists but is empty will cause 0 to be
 * returned.
 */
extern int config_parse(const char *path, struct config_map tab[], bool fail);

#endif /* LIBCASSAVA_CONFIG_KV_H */

/* vim: set cin ts=4 sw=4 et: */
