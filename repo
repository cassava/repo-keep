#!/bin/bash
# File: repo-update
# Desc: repo-update is a supplement to repo-add, in that it deletes any
# older found packages and automatically updates the repository database.

# Copyright (c) 2010–2011 Ben Morgan <neembi@googlemail.com>
# 
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
# 
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

ERR_CMD=1
ERR_REPO=2
ERR_PKG=4
ERR_CONFIG=8

VERSION="1.5.0 (15. June 2011)"
PKGEXT="(gz|bz2|xz)"
PKG_STRICT="^$2-[a-z0-9._]+-[0-9]+-(any|i686|x86_64).pkg.tar.$PKGEXT$"
PKG_LENIENT="^$2-.+-[0-9]+-(any|i686|x86_64).pkg.tar.$PKGEXT$"

repo_dir=
repo_db=

function _version() {
    echo "repo version $VERSION"
}

function _help() {
    _version
    echo "Usage: repo <command> [arguments]"
    echo
    echo "Commands available:"
    echo "    sync              compare packages in the database to AUR for new versions."
    echo "    add <pkgname>     add the package with <pkgname> to the database, by"
    echo "                      finding in the same directory the latest file for that"
    echo "                      package, deleting the others, and updating the database."
    echo "    update            same as add, except scan automatically for changed packages."
    echo "    remove <pkgname>  remove the pacakge with <pkgname> from the database, by"
    echo "                      removing the entry from the database and deleting the files."
    echo ""
    echo "Note that in all of these cases, <pkgname> is the name of the package, without"
    echo "anything else. Eg. pacman and not pacman-3.5.3-1-i686.pkg.tar.xz"
}

function _load_config() {
    if [ -f $HOME/.repo-update ]; then
        . $HOME/.repo-update
    else
        echo "error (config): config file $HOME/.repo-update missing."
        echo 'creating config file with default values...'
        echo 'repo_dir="$HOME/packages"' > $HOME/.repo-update
        echo 'repo_db="local.db.tar.gz"' >> $HOME/.repo-update
        echo 'please edit the configuration file before using repo-update again.'
        exit $ERR_CONFIG
    fi

    if [ -z $repo_dir ]; then
        echo 'error (config): repo_dir variable missing.'
        die=true
    fi
    if [ -z $repo_db ]; then
        echo 'error (config): repo_db variable missing.'
        die=true
    fi
    [[ $die ]] && exit $ERR_CONFIG
}

function _check() {
    # $1 is the directory where the repository is situated
    # $2 is the database filename
    if [ ! -d $1 ]; then
        echo "error: directory $1 does not exist."
        exit $ERR_REPO
    else
        cd $1
    fi
    if [ ! -f $2 ]; then
        echo "error: default repository database '$2' does not exist."
        exit $ERR_REPO
    fi
}

function add() {
    # $1 is the directory where the repository is situated
    # $2 is the database filename
    # $3 is the package name
    _check $1 $2
    
    # package filename format
    list=($(ls -1v | egrep "$strict"))
    amount=${#list[*]}
    last=$(($amount -1))

    echo "database is $1"
    echo "found $amount packages"
    if [ $amount == 0 ]; then
        echo "error: package $2 does not exist."
        exit $ERR_PKG
    elif [ $amount == 1 ]; then
        echo "adding ${list[$last]}"
        repo-add $1 ${list[$last]}
    elif [[ $amount -ge 2 ]]; then
        for i in $(seq 0 $(($amount - 2))); do
            echo "deleting ${list[$i]}"
            rm ${list[$i]}
        done
        echo "adding ${list[$last]}"
        repo-add $1 ${list[$last]}
    fi
}

load_config
if [ $# == 1 ]; then
    case $1 in
        -h|--help) help
                   exit;;
               -*) help
                   exit $ERR_CMD;;
                *) update $repo_db $1;;
    esac
else
    help
    exit $ERR_CMD
fi

