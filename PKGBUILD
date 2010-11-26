# Contributor: Ben Morgan <uv.sound@gmail.com>
pkgname=repo-update
pkgver=1.4
pkgrel=1
pkgdesc="repo-update is a supplement to repo-add which simplifies updating local repositories"
arch=('any')
url="https://github.com/cassava/repo-update"
license=('ISC')
groups=()
depends=(pacman)
makedepends=()
provides=()
conflicts=()
replaces=()
backup=()
install=
source=(https://github.com/downloads/cassava/$pkgname/$pkgname-$pkgver.tar.gz)
noextract=()

build() {
  mkdir -p $pkgdir/usr/bin
  install -m 0755 $srcdir/repo-update $pkgdir/usr/bin/
}
