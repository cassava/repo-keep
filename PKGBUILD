# Maintainer: Ben Morgan <neembi@googlemail.com>
pkgname=repo-keep
pkgver=1.9.1
pkgrel=1
pkgdesc="A supplement to repo-add and repo-remove which simplifies updating local repositories"
arch=('i686 x86_64')
url="https://github.com/cassava/repo-keep"
license=('MIT')
depends=(pacman)
source=(https://github.com/downloads/cassava/$pkgname/$pkgname-$pkgver.tar.gz)

build() {
  cd $srcdir/$pkgname-$pkgver
  ./configure --prefix="$pkgdir/usr" || return 1
  make || return 1
}

package() {
  cd $srcdir/$pkgname-$pkgver

  # Install repo program
  make install

  # Install other documentation
  install -m644 TODO README NEWS $pkgdir/usr/share/doc/repo-keep/

  # Install completion files
  mkdir -p $pkgdir/usr/share/zsh/site-functions/
  install -m644 contrib/zsh_completion $pkgdir/usr/share/zsh/site-functions/_repo

}

md5sums=('750b01e1d2a92768945cf4bda6f337eb')
