/*
 *  This file is part of XeroLinux Settings Manager.
 *
 *  Ramon Buldó <ramon@manjaro.org>
 *
 *  XeroLinux Settings Manager is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  XeroLinux Settings Manager is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XeroLinux Settings Manager.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NOTIFIER_H
#define NOTIFIER_H

#include "LanguagePackagesItem.h"
#include "NotifierSettingsDialog.h"

#include <QtCore/QTimer>
#include <KNotifications/KStatusNotifierItem>

class Notifier : public QObject
{
    Q_OBJECT

public:
    explicit Notifier( QObject* parent = 0 );
    virtual ~Notifier();

private:
    KStatusNotifierItem* m_tray;
    QTimer* m_timer;
    bool m_checkLanguagePackage;
    bool m_checkKernel;
    bool m_checkUnsupportedKernel;
    bool m_checkUnsupportedKernelRunning;
    bool m_checkNewKernel;
    bool m_checkNewKernelLts;
    bool m_checkNewKernelRecommended;

    void cLanguagePackage();
    void cKernel();
    void showNewKernelNotification();
    bool isPackageIgnored( const QString package, const QString group );
    void addToConfig( const QString package, const QString group );

protected slots:
    void loadConfiguration();

};

#endif // NOTIFIER_H
