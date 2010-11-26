REPO-UPDATE README
=======================================================================

**Repo-Update** is a supplement to `repo-add` which simplifies updating local
repositories. It is _not_ a suplement to `repo-remove` however.

Repo-Update is distributed under the ISC License (see `LICENSE`).


### Limitations

I am not aware of any other scripts which already provide what Repo-Update
provides (though there may have been one created since the writing of
this). If there is one, please let me know.

As of yet, Repo-Update does not however provide the following features:

  * automatic removal of packages
  * multiple repositories

If you need these features or more, and you would like to modify Repo-Update,
feel free to do so (as it is licensed under the ISC). If you want to send me
your modifications, I would also be grateful.


### Usage
Once you have created a configuration file (just run `repo-update` and it will
create one for you; you need to edit the config afterwards though), Repo-Update
is mostly used in the following way (which is an example):

    $ makepkg -c
      [...]
    $ repo-update is-net
    database is local.db.tar.gz
    found 2 packages
    deleting is-net-0.9.1-1-any.pkg.tar.xz
    adding is-net-0.9.2-1-any.pkg.tar.xz
    ==> Extracting database to a temporary location...
    ==> Adding package 'is-net-0.9.2-1-any.pkg.tar.xz'
      -> Removing existing entry 'is-net-0.9.1-1'...
      -> Creating 'desc' db entry...
      -> Computing md5 checksums...
      -> Creating 'depends' db entry...
    ==> Creating updated database file 'local.db.tar.gz'


### Repo-Update Configuration File Example
The configuration file is located at `~/.repo-update`.

    repo_dir="/home/abs/packages"
    repo_db="local.db.tar.gz"


### Tips
Repo-Update makes most sense (to me) if you have a location where all your
built packages end up (see `/etc/makepkg.conf`). Then you would do something
like this:

    $ makepkg -c
      [...]
    $ repo-update <package_name>
      [...]

