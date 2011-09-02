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

#include "repo.h"

#include <stdio.h>
#include <string.h>
#include <argp.h>

// Variables and constants for argp argument parsing.
const char *argp_program_version = REPO_VERSION_STRING;
const char *argp_program_bug_address = "<neembi@googlemail.com>";

static char args_doc[] = "<COMMAND> [PACKAGES ...]";
static char doc[] =
    "Manage local pacman repositories.\n"
    "\n"
    "Commands available:\n"
    "  sync             Compare packages in the database to AUR for new versions.\n"
    "  add <pkgname>    Add the package(s) with <pkgname> to the database by\n"
    "                   finding in the same directory of the database the latest\n"
    "                   file for that package (by file date and by version number),\n"
    "                   deleting the others, and updating the database.\n"
    "  update           Same as add, except scan and add changed packages.\n"
    "  remove <pkgname> Remove the package with <pkgname> from the database, by\n"
    "                   removing the entry from the database and deleting the\n"
    "                   files.\n"
    "\n"
    "NOTE: In all of these cases, <pkgname> is the name of the package, without\n"
    "anything else. For example: pacman, and not pacman-3.5.3-1-i386.pkg.tar.xz";

static struct argp_option options[] = {
    {"soft",    's', 0,        0, "Don't delete any files (n/a for: sync)"},
    {"natural", 'n', 0,        0, "Don't take package creation time into account"},
    {"quiet",   'q', 0,        0, "Don't produce unnecessary output"},
    {"verbose", 'v', 0,        0, "Tell me more, tell me more!"},
    {"config",  'c', "CONFIG", 0, "Alternate configuration file"},
    { 0 }
};

struct arguments
{
    int soft;               // don't delete files
    int natural;            // don't compare file creation times
    int quiet;              // don't print any unnecessary output
    int verbose;            // tell me more!
    char *config;           // configuration file where next two values are stored
    char *db_name;          // config::database name
    char *db_path;          // config::path to db location (with packages)
    char *command;          // command to execute (one of: sync, update, add, remove)
    char *args[ARG_BUFFER]; // holds pointers to package arguments
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
                arguments->args[state->arg_num-1] = arg;
                // TODO: is this necessary?
                arguments->args[state->arg_num] = NULL;
            }
            break;
        case ARGP_KEY_END:
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

static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char **argv)
{
    struct arguments arguments;
    
    // set default values
    arguments.soft = 0;
    arguments.natural = 0;
    arguments.quiet = 0;
    arguments.verbose = 0;
    arguments.config = CONFIG_PATH;

    // parse the command line arguments
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    // read the configuration file
    //config_parse(arguments->config);

    return 0;
}

// vim: set ts=4 sw=4 expandtab:
