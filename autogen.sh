# Setup the autotools system (after autoclean.sh for example)

srcdir=/home/benmorgan/code/repo-keep
if [ "$PWD" != "$srcdir" ]; then
    echo "Can only execute this from $srcdir!"
    exit 1
fi


mkdir config
aclocal
automake --add-missing
autoconf
