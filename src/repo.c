/*
 * repo.c
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
#include "lib/config_bm.h"
#include "lib/util_bm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>

// Variables and constants for argp argument parsing.
const char *argp_program_version = REPO_VERSION_STRING;
const char *argp_program_bug_address = "<neembi@googlemail.com>";

static char args_doc[] = "<add|remove|update|sync> [PACKAGES ...]";
static char doc[] =
    "Manage local pacman repositories.\n"
    "\n"
    "Commands available:\n"
    "  add <pkgname>    Add the package(s) with <pkgname> to the database by\n"
    "                   finding in the same directory of the database the latest\n"
    "                   file for that package (by file date and by version number),\n"
    "                   deleting the others, and updating the database.\n"
    "  remove <pkgname> Remove the package with <pkgname> from the database, by\n"
    "                   removing the entry from the database and deleting the\n"
    "                   files.\n"
    "  sync             Compare packages in the database to AUR for new versions.\n"
    "  update           Same as add, except scan and add changed packages.\n"
    "\n"
    "NOTE: In all of these cases, <pkgname> is the name of the package, without\n"
    "anything else. For example: pacman, and not pacman-3.5.3-1-i386.pkg.tar.xz";

static struct argp_option options[] = {
  // long       key  arg       ?  description
    {"soft",    's', NULL,     0, "Don't delete any files (n/a for: sync)"},
    {"natural", 'n', NULL,     0, "Don't take package creation time into account"},
    {"quiet",   'q', NULL,     0, "Don't produce unnecessary output"},
    {"verbose", 'v', NULL,     0, "Tell me more, tell me more!"},
    {"config",  'c', "CONFIG", 0, "Alternate configuration file"},
    { NULL }
};

static struct config_map configuration[] = {
    { "db_name", NULL },
    { "db_dir", NULL },
    { NULL, NULL }
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch(key) {
        case 's': // soft
            arguments->soft = 1;
            break;
        case 'n': // natural
            arguments->natural = 1;
            break;
        case 'q': // quiet
            arguments->quiet = 1;
            arguments->verbose = 0;
            break;
        case 'v': // verbose
            arguments->verbose = 1;
            arguments->quiet = 0;
            break;
        case 'c': // alternative config
            arguments->config = arg;
            break;
        case ARGP_KEY_ARG:
            #define _argeq(S)  !strcmp(arg, S)
            if (state->arg_num == 0) {
                if (_argeq("add") || _argeq("update") || _argeq("sync") || _argeq("remove"))
                    arguments->command = arg;
                else
                    argp_usage(state);
            } else {
                arguments->argv[state->arg_num-1] = arg;
            }
            break;
        case ARGP_KEY_END:
            arguments->argc = state->arg_num;
            // Make sure that the amount of arguments is correct
            if ( (state->arg_num < 1) ||
                 ( (!strcmp(arguments->command, "update")
                   || !strcmp(arguments->command, "sync"))
                   && (state->arg_num > 1) ) ||
                 ( (!strcmp(arguments->command, "add")
                   || !strcmp(arguments->command, "remove"))
                   && (state->arg_num == 1) ) )
                argp_usage(state);
        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

static char *tildestr(char *line)
{
    char *home;
    
    if (*line == '~') {
        home = getenv("HOME");
        return strcatbm(home, line+1);
    }
    return line;
}


int main(int argc, char **argv)
{
    struct arguments arguments;
    struct argp argp = {options, parse_opt, args_doc, doc};
    char *default_config = tildestr(CONFIG_PATH);
    int i, ret;
    
    // set default values
    arguments.soft = 0;
    arguments.natural = 0;
    arguments.quiet = 0;
    arguments.verbose = 0;
    arguments.config = default_config;

    // parse the command line arguments
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    // read the configuration file
    ret = config_parse(arguments.config, configuration, CONFIG_FAIL);
    for (i = 0; i < CONFIG_LEN; i++)
        if (configuration[i].value == NULL) {
            fprintf(stderr, "error: required value for key '%s' missing from configuration file\n",
                    configuration[i].key);
            ret = -1;
        }
    if (ret == -1) { // there was no configuration file or a problem with the values
        fprintf(stderr, "error: repo requires a configuration file, by default located at\n"
                        "           %s\n"
                        "       with at least the following lines:\n"
                        "           db_name = name_of_database.db.tar.gz\n"
                        "           db_dir  = /path/to/database/\n", default_config);
        exit(ERR_CONFIG);
    }
    arguments.db_name = configuration[0].value;
    arguments.db_dir = configuration[1].value;
    arguments.db_path = strcatbm(arguments.db_dir, arguments.db_name);

    // perform the given action by switching on first character
    switch (*arguments.command) {
        case 'a':
            repo_add(&arguments);
            break;
        case 'u':
            repo_update(&arguments);
            break;
        case 's':
            repo_sync(&arguments);
            break;
        case 'r':
            repo_remove(&arguments);
            break;
        default:
            // should never happen
            fprintf(stderr, "error: this is an error that should never happen!\n");
            exit(ERR_UNDEF);
    }

    // finally
    free(default_config);
    free(arguments.db_name);
    free(arguments.db_dir);
    free(arguments.db_path);

    return 0;
}

// vim: set ts=4 sw=4 et:
