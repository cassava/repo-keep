# Bring everything to a clean state.

srcdir=/home/benmorgan/code/repo-keep
if [ "$PWD" != "$srcdir" ]; then
    echo "Can only execute this from $srcdir!"
    exit 1
fi


# ./
rm aclocal.m4
rm -r autom4te.cache
rm -r config{,.log,.status}
rm configure
rm Makefile{,.in}

# contrib/
rm doc/Makefile{,.in}

# doc/
rm doc/Makefile{,.in}

# src/
rm src/Makefile{,.in}
rm src/*.o
rm src/repo
rm -r src/.deps
