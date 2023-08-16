# Maintainer : dr460nf1r3 <dr460nf1r3 at xerolinux dot org>
# Contributor : librewish <librewish@xerolinux.org>
# Contributor : Ramon Buldo <ramon@manjaro.org>

pkgbase=xero-settings-manager
pkgname=('xero-settings-manager' 'xero-settings-manager-kcm'
         'xero-settings-manager-notifier' 'xero-settings-manager-knotifier')
pkgver=1.0.3
pkgrel=1
pkgdesc="Xero Linux system settings (Manjaro settings manager ported to work with Arch standards and limited to only DKMS drivers)"
arch=('i686' 'x86_64')
url="https://github.com/xerolinux/$pkgbase"
license=("GPL")
depends=('icu' 'qt5-base>=5.12.3' 'hwinfo' 'kitemmodels' 'kauth'
         'kcoreaddons' 'ckbcomp' 'xdg-utils' 'mhwd-xero-git')
optdepends=('xero-settings-manager-notifier-git: qt-based'
            'xero-settings-manager-knotifier-git: knotifications-based')
makedepends=('git' 'extra-cmake-modules' 'kdoctools' 'qt5-tools' 'knotifications'
             'kconfigwidgets' 'kcmutils')
conflicts=('kcm-msm')
source=("https://gitlab.com/xero-linux/applications/$pkgbase/-/archive/$pkgver/xero-settings-manager-$pkgver.tar.gz")
sha256sums=('SKIP')

build() {
  cd "$srcdir/$pkgbase-$pkgver"
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
  provides=($pkgbase)
  conflicts=($pkgbase)
  cd "$srcdir/$pkgbase-$pkgver/build"
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
  pkgdesc="Xero Linux system settings - KCM for Plasma 5"
  depends=('xero-settings-manager' 'kcmutils' 'kconfigwidgets')
  replaces=('kcm-msm')
  cd "$srcdir/$pkgbase-$pkgver/build"
  make DESTDIR=${pkgdir} install
  rm -rf $pkgdir/etc  
  rm -rf $pkgdir/usr/bin
  rm -rf $pkgdir/usr/lib/kauth
  rm -rf $pkgdir/usr/share/{applications,dbus-1,icons,polkit-1}
}

package_xero-settings-manager-notifier() {
  pkgdesc="Xero Linux system settings - notifier"
  depends=('xero-settings-manager')
  provides=('xero-settings-manager-kde-notifier')
  conflicts=('xero-settings-manager-kde-notifier')
  cd "$srcdir/$pkgbase-$pkgver/build"
  make DESTDIR=${pkgdir} install
  rm -rf $pkgdir/etc/dbus-1
  rm -rf $pkgdir/etc/xdg/autostart/msm_kde_notifier.desktop
  rm -rf $pkgdir/usr/lib/
  rm -rf $pkgdir/usr/share/{kservices5,dbus-1,icons,polkit-1}
  rm -rf $pkgdir/usr/share/applications/xero*
  rm -rf $pkgdir/usr/share/applications/msm_kde_notifier_settings.desktop
  rm -rf $pkgdir/usr/bin/xero*
  rm -rf $pkgdir/usr/bin/msm_kde_notifier
}

package_xero-settings-manager-knotifier() {
  pkgdesc="Xero Linux system settings - knotifier"
  depends=('xero-settings-manager' 'knotifications')
  conflicts=('xero-settings-manager-notifier')
  replaces=('xero-settings-manager-kde-notifier')
  cd "$srcdir/$pkgbase-$pkgver/build"
  make DESTDIR=${pkgdir} install
  rm -rf $pkgdir/etc/dbus-1
  rm -rf $pkgdir/etc/xdg/autostart/msm_notifier.desktop
  rm -rf $pkgdir/usr/lib/
  rm -rf $pkgdir/usr/share/{kservices5,dbus-1,icons,polkit-1}
  rm -rf $pkgdir/usr/share/applications/xero*
  rm -rf $pkgdir/usr/share/applications/msm_notifier_settings.desktop
  rm -rf $pkgdir/usr/bin/xero*
  rm -rf $pkgdir/usr/bin/msm_notifier
} 
