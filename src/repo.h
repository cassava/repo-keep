/*
 * repo.h
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

#ifndef REPO_H
#define REPO_H

#define REPO_VERSION    "2.0"
#define REPO_DATE       "1. September 2011"
#define REPO_VERSION_STRING "repo " REPO_VERSION "  (" REPO_DATE ")"

#define ERROR_CMD       1
#define ERROR_REPO      2
#define ERROR_PKG       4
#define ERROR_CONFIG    8

#define ARG_BUFFER      1024
#define CONFIG_PATH     "~/.repo.conf"

#define PKG_STRICT_EXT  "-[a-z0-9._]+-[0-9]+-(any|i686|x86_64).pkg.tar.(gz|bz2|xz)"
#define PKG_LENIENT_EXT "-.+-[0-9]+-(any|i686|x86_64).pkg.tar.(gz|bz2|xz)"

#endif // REPO_H
