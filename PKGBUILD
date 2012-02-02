# Maintainer: Ben Morgan <neembi@googlemail.com>
pkgname=repo-keep
pkgver=1.0
pkgrel=1
pkgdesc="A supplement to repo-add and repo-remove which simplifies updating local repositories"
arch=('i686 x86_64')
url="https://github.com/cassava/repo-keep"
license=('MIT')
depends=(pacman)
source=(https://github.com/downloads/cassava/$pkgname/$pkgname-$pkgver.tar.gz)

build() {
  cd $srcdir/$pkgname-$pkgver

  make
}

package() {
  cd $srcdir/$pkgname-$pkgver

  make install

  # Install the script
  install -Dm755 repo-update $pkgdir/usr/bin/repo-update

  # Install the license and other documentation
  install -d $pkgdir/usr/share/doc/repo-update
  install -m644 TODO CHANGES $pkgdir/usr/share/doc/repo-update/

  # Install completion files
  mkdir -p $pkgdir/usr/share/zsh/site-functions/
  install -m644 contrib/zsh_completion $pkgdir/usr/share/zsh/site-functions/_repo

}
