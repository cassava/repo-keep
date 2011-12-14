# Maintainer: Ben Morgan <neembi@googlemail.com>
pkgname=repo-keep
pkgver=2.0
pkgrel=1
pkgdesc="A supplement to repo-add and repo-remove which simplifies updating local repositories"
arch=('any')
url="https://github.com/cassava/repo-update"
license=('MIT')
depends=(pacman)
source=(https://github.com/downloads/cassava/$pkgname/$pkgname-$pkgver.tar.gz)

build() {
  cd $srcdir/$pkgname-$pkgver

  make
}

package() {
  cd $srcdir/$pkgname-$pkgver

  # Install the script
  install -Dm755 repo-update $pkgdir/usr/bin/repo-update

  # Install the license and other documentation
  install -Dm644 LICENSE $pkgdir/usr/share/licenses/repo-update/LICENSE
  install -d $pkgdir/usr/share/doc/repo-update
  install -m644 TODO CHANGES $pkgdir/usr/share/doc/repo-update/

  # Install the completion files
  install -Dm644 zsh-completion $pkgdir/usr/share/zsh/site-functions/_repo-update
}

md5sums=('2cf63cb355bccbe6eda52cf79dbe7b24')
