# Maintainer : Ramon Buldo <ramon@manjaro.org>

pkgbase=xero-settings-manager
pkgname=('xero-settings-manager' 'xero-settings-manager-kcm'
         'xero-settings-manager-notifier' 'xero-settings-manager-knotifier')
pkgver=0.5.6
#_commit=e085b661c3c97e8aa39a5b49896a88a95d84caf9
pkgrel=1
pkgdesc="Manjaro Linux System Settings Tool"
arch=('i686' 'x86_64')
url="https://github.com/xerolinux/xero-settings-manager"
license=("GPL")
depends=('icu<64.2' 'qt5-base>=5.12.3' 'hwinfo' 'kitemmodels' 'kauth' 
         'kcoreaddons' 'ckbcomp' 'xdg-utils')
optdepends=('xero-settings-manager-notifier: qt-based'
            'xero-settings-manager-knotifier: knotifications-based')
makedepends=('extra-cmake-modules' 'kdoctools' 'qt5-tools' 'knotifications' 
             'kconfigwidgets' 'kcmutils')
conflicts=('kcm-msm')
# source=("msm-$pkgver-$pkgrel.tar.gz::$url/-/archive/$_commit/$pkgname-$_commit.tar.gz")
source=("msm-$pkgver-$pkgrel.tar.gz::$url/-/archive/$pkgver/$pkgbase-$pkgver.tar.gz")
sha256sums=('2cebe2485563c9bb9445df0ae7644fdcfa6d0ac3167cf0205c5a785f8520f376')

prepare() {
#  mv ${pkgbase}-${_commit} ${pkgbase}-${pkgver}
  cd "$srcdir/${pkgbase}-${pkgver}"
  # patches here
}

build() {
  cd "$srcdir/${pkgbase}-${pkgver}"
  mkdir -p build
  cd build
  cmake ../ \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DLIB_INSTALL_DIR=lib \
    -DKDE_INSTALL_USE_QT_SYS_PATHS=ON \
    -DSYSCONF_INSTALL_DIR=/etc
  CXXFLAGS+="-std=gnu++98" make
}

package_xero-settings-manager() {
  cd "$srcdir/${pkgbase}-${pkgver}/build"
  make DESTDIR=${pkgdir} install 
  rm -rf $pkgdir/usr/bin/msm_notifier
  rm -rf $pkgdir/usr/bin/msm_kde_notifier
  rm -rf $pkgdir/usr/lib/qt
  rm -rf $pkgdir/usr/share/kservices5
  rm -rf $pkgdir/usr/share/applications/msm_notifier_settings.desktop
  rm -rf $pkgdir/usr/share/applications/msm_kde_notifier_settings.desktop
  rm -rf $pkgdir/etc/xdg
}

package_xero-settings-manager-kcm() {
  pkgdesc="Manjaro Linux System Settings Tool (KCM for Plasma 5)"
  depends=('xero-settings-manager' 'kcmutils' 'kconfigwidgets')
  replaces=('kcm-msm')
  cd "$srcdir/${pkgbase}-${pkgver}/build"
  make DESTDIR=${pkgdir} install
  rm -rf $pkgdir/etc  
  rm -rf $pkgdir/usr/bin
  rm -rf $pkgdir/usr/lib/kauth
  rm -rf $pkgdir/usr/share/{applications,dbus-1,icons,polkit-1}
}

package_xero-settings-manager-notifier() {
  pkgdesc="Manjaro Linux System Settings Tool (Notifier)"
  depends=('xero-settings-manager')
  provides=('xero-settings-manager-kde-notifier')
  conflicts=('xero-settings-manager-kde-notifier')
  cd "$srcdir/${pkgbase}-${pkgver}/build"
  make DESTDIR=${pkgdir} install
  rm -rf $pkgdir/etc/dbus-1
  rm -rf $pkgdir/etc/xdg/autostart/msm_kde_notifier.desktop
  rm -rf $pkgdir/usr/lib/
  rm -rf $pkgdir/usr/share/{kservices5,dbus-1,icons,polkit-1}
  rm -rf $pkgdir/usr/share/applications/manjaro*
  rm -rf $pkgdir/usr/share/applications/msm_kde_notifier_settings.desktop
  rm -rf $pkgdir/usr/bin/manjaro*
  rm -rf $pkgdir/usr/bin/msm_kde_notifier
}

package_xero-settings-manager-knotifier() {
  pkgdesc="Manjaro Linux System Settings Tool (Notifier for Plasma 5)"
  depends=('xero-settings-manager' 'knotifications')
  conflicts=('xero-settings-manager-notifier')
  replaces=('xero-settings-manager-kde-notifier')
  cd "$srcdir/${pkgbase}-${pkgver}/build"
  make DESTDIR=${pkgdir} install
  rm -rf $pkgdir/etc/dbus-1
  rm -rf $pkgdir/etc/xdg/autostart/msm_notifier.desktop
  rm -rf $pkgdir/usr/lib/
  rm -rf $pkgdir/usr/share/{kservices5,dbus-1,icons,polkit-1}
  rm -rf $pkgdir/usr/share/applications/manjaro*
  rm -rf $pkgdir/usr/share/applications/msm_notifier_settings.desktop
  rm -rf $pkgdir/usr/bin/manjaro*
  rm -rf $pkgdir/usr/bin/msm_notifier
} 
