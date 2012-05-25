/*
 * actions.h
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

#ifndef ACTIONS_H
#define ACTIONS_H

#include "repo.h"

/*
 * repo_list: list all the packages registered in the database.
 */
extern int repo_list(Arguments *);

/*
 * repo_add: add the packages given in *arg from the database,
 * remove old packages from the filesystem (if !arg->soft).
 */
extern int repo_add(Arguments *);

/*
 * repo_remove: remove the packages given in *arg from the database and maybe
 * also from the filesystem (if !arg->soft).
 */
extern int repo_remove(Arguments *);

/*
 * repo_update: scan the directory where the database resides,
 * and add all packages that are newer than the database.
 */
extern int repo_update(Arguments *);

/*
 * repo_sync: ...
 */
extern int repo_sync(Arguments *);

#endif // ACTIONS_H

/* vim: set cin ts=4 sw=4 et: */
