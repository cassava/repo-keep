/*
 * update.c
 * Includes the code for the commands 'add', 'remove' and 'update'.
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

#include "common.h"
#include "bm_string.h"
#include "bm_util.h"
#include "bm_list.h"
#include <stdio.h>
#include <stdlib.h>

#define DRYRUN


void repo_update(Arguments *arg)
{
    repo_check(arg);
}

void repo_add(Arguments *arg)
{
    repo_check(arg);
}

/*
 * repo_remove: remove the packages given in *arg from the database and maybe
 * also from the filesystem (if !arg->soft). If an errors are encountered, an
 * error message is printed and the operation is aborted.
 */
void repo_remove(Arguments *arg)
{
    char *cmd, *argstr;

    repo_check(arg);

    /* remove entry from database */
    argstr = bm_strjoin(arg->argv, arg->argc, " ", 0);
    cmd = bm_strvcat(SYSTEM_REPO_REMOVE, " ", arg->db_path, " ", argstr, NULL);
    if (arg->verbose) puts(cmd);
    system(cmd);

    free(cmd);
    free(argstr);
    
    /* if files should be removed, remove files */
    if (!arg->soft) {
        char *regex, *mesg;
        Node *head;

        argstr = bm_strjoin(arg->argv, arg->argc, "|", 0);
        regex = bm_strvcat("^(", argstr, ")", PKG_EXT, NULL);
        head = get_regex_files(regex, arg->db_dir);
        free(argstr);
        free(regex);

        argstr = list_strjoin(head, "\n              ");
        mesg = bm_strvcat("Delete files: ", argstr, "?", NULL);
        free(argstr);

        if (confirm(mesg, 1)) {
            argstr = list_strjoin(head, " ");
            cmd = bm_strvcat("cd ", arg->db_dir, " && rm ", argstr, NULL);
            free(argstr);

            system(cmd);
            free(cmd);
        }

        free(mesg);
        list_free_all(head);
    }
}

