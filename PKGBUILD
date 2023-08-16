# Maintainer: Librewish <librewish@gmail.com>
# Contributer : Ramon Buldo <ramon@xero.org>

pkgbase=xero-settings-manager-git
_pkgbase=xero-settings-manager
pkgname=('xero-settings-manager-git' 'xero-settings-manager-kcm-git'
         'xero-settings-manager-notifier-git' 'xero-settings-manager-knotifier-git')
pkgver=1.0
pkgrel=1
pkgdesc="Xero Linux system settings (Manjaro settings manager ported to work with Arch standards and limited to only DKMS drivers)"
arch=('i686' 'x86_64')
url="https://github.com/xerolinux/xero-settings-manager"
license=("GPL")
depends=('icu' 'qt5-base>=5.12.3' 'hwinfo' 'kitemmodels' 'kauth' 
         'kcoreaddons' 'ckbcomp' 'xdg-utils' 'mhwd-xero-git')
optdepends=('xero-settings-manager-notifier-git: qt-based'
            'xero-settings-manager-knotifier-git: knotifications-based')
makedepends=('git' 'extra-cmake-modules' 'kdoctools' 'qt5-tools' 'knotifications' 
             'kconfigwidgets' 'kcmutils')
conflicts=('kcm-msm')
source=("git+$url.git")
sha256sums=('SKIP')

pkgver() {
    cd "$srcdir/$_pkgbase"
    git describe --long --tags | sed 's/\([^-]*-g\)/r\1/;s/-/./g'
}


build() {
  cd "$srcdir/$_pkgbase"
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

package_xero-settings-manager-git() {
  provides=('xero-settings-manager')
  conflicts=('xero-settings-manager')

  cd "$srcdir/$_pkgbase/build"
  make DESTDIR=${pkgdir} install 
  rm -rf $pkgdir/usr/bin/msm_notifier
  rm -rf $pkgdir/usr/bin/msm_kde_notifier
  rm -rf $pkgdir/usr/lib/qt
  rm -rf $pkgdir/usr/share/kservices5
  rm -rf $pkgdir/usr/share/applications/msm_notifier_settings.desktop
  rm -rf $pkgdir/usr/share/applications/msm_kde_notifier_settings.desktop
  rm -rf $pkgdir/etc/xdg
}

package_xero-settings-manager-kcm-git() {
  pkgdesc="Xero Linux system settings - Plasma 5 KCM"
  depends=('xero-settings-manager' 'kcmutils' 'kconfigwidgets')
  provides=('xero-settings-manager-kcm')
  conflicts=('xero-settings-manager-kcm')

  cd "$srcdir/$_pkgbase/build"
  make DESTDIR=${pkgdir} install
  rm -rf $pkgdir/etc  
  rm -rf $pkgdir/usr/bin
  rm -rf $pkgdir/usr/lib/kauth
  rm -rf $pkgdir/usr/share/{applications,dbus-1,icons,polkit-1}
}

package_xero-settings-manager-notifier-git() {
  pkgdesc="Xero Linux system settings - notifier"
  depends=('xero-settings-manager')
  provides=('xero-settings-manager-notifier')
  conflicts=('xero-settings-manager-notifier')

  cd "$srcdir/$_pkgbase/build"
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

package_xero-settings-manager-knotifier-git() {
  pkgdesc="Xero Linux system settings - knotifier"
  depends=('xero-settings-manager' 'knotifications')
  provides=('xero-settings-manager-knotifier')
  conflicts=('xero-settings-manager-knotifier')

  cd "$srcdir/$_pkgbase/build"
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
