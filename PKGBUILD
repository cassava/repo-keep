# Contributor: Ben Morgan <benm.morgan@gmail.com>
pkgname=repo-update
pkgver=1.4
pkgrel=1
pkgdesc="repo-update is a supplement to repo-add which simplifies updating local repositories"
arch=('any')
url=""
license=('ISC')
groups=()
depends=()
makedepends=()
provides=()
conflicts=()
replaces=()
backup=()
install=
source=($pkgname)
noextract=()

build() {
  mkdir -p $pkgdir/usr/bin
  install -m 0755 $srcdir/repo-update $pkgdir/usr/bin/
}
