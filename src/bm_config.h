/*
 * config_bm.h
 * For parsing a simple key-value configuration file.
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

#ifndef BMCONFIG_H
#define BMCONFIG_H

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
 * Returns:  0 = no problems detected
 *          -1 = file described by path could not be opened
 *           1 = (if fail != 0) on invalid line encountered
 *           2 = (if fail != 0) on invalid key encountered
 * Note: You must check tab yourself to see if any values have
 * changed. If you have values set to NULL, they may still be
 * NULL if the key was not found in the config file. All to say,
 * a config file that exists but is empty will cause 0 to be
 * returned.
 */
extern int config_parse(const char *path, struct config_map tab[], int fail);

#endif // BMCONFIG_H
