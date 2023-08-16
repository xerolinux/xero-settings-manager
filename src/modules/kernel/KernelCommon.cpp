/*
 *  This file is part of Garuda Settings Manager.
 *
 *  Ramon Buldó <ramon@manjaro.org>
 *
 *  Garuda Settings Manager is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Garuda Settings Manager is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Garuda Settings Manager.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <limits>
#include "ActionDialog.h"
#include "KernelCommon.h"
#include "KernelInfoDialog.h"

#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QDebug>
#include <QProcess>


QString
KernelCommon::getName()
{
    return QString( "msm_kernel" );
}


QString
KernelCommon::getTitle()
{
    return QString( tr ( "Kernel" ) );
}


QString
KernelCommon::getDescription()
{
    return QString( tr ( "Add and remove kernels" ) );
}


void
KernelCommon::load( KernelModel* kernelModel )
{
    kernelModel->update();
}


void
KernelCommon::kernelAction( const QModelIndex& index )
{
    bool isInstalled = qvariant_cast<bool>( index.data( KernelModel::IsInstalledRole ) );
    bool isRunning = qvariant_cast<bool>( index.data( KernelModel::IsRunningRole ) );
    if ( isRunning && isInstalled )
    {
        QMessageBox msgBox;
        msgBox.setText("The Linux  packages is running. so removal is not recommended.");
        msgBox.exec();
    }

    else if ( isInstalled )
    {
        removeKernel( index );
    }
    else
    {
        installKernel( index );
    }
}


void
KernelCommon::installKernel( const QModelIndex& index )
{
    QStringList modules = qvariant_cast<QStringList>( index.data( KernelModel::InstalledModulesRole ) );
    QString kernel = qvariant_cast<QString>( index.data( KernelModel::PackageRole ) );
    QString version = qvariant_cast<QString>( index.data( KernelModel::VersionRole ) );
    QString kernelh = qvariant_cast<QString>( index.data( KernelModel::PackageRole ) );
    QString headers = kernelh.append("-headers");
    QStringList packageList;
    packageList << kernel << modules << headers;

    QString title = QString( tr( "Install Linux %1" ) ).arg( version );
    QString message = QString( tr( "New Kernel package(s) is/are ready to install. \nWould you like to continue?" ) );

    QString information = QString( tr( "The following packages will be installed:\n" ) );
    foreach ( const QString p, packageList )
    {
        information.append( p );
        information.append( "\n" );
    }

    QStringList arguments;
    arguments << "--noconfirm" << "--noprogressbar" << "-S" << packageList;
    QVariantMap args;
    args["arguments"] = arguments;
    KAuth::Action installAction( QLatin1String( "org.garuda.msm.kernel.install" ) );
    installAction.setHelperId( QLatin1String( "org.garuda.msm.kernel" ) );
    installAction.setArguments( args );
    installAction.setTimeout( std::numeric_limits<int>::max() );

    ActionDialog actionDialog;
    actionDialog.setInstallAction( installAction );
    actionDialog.setWindowTitle( title );
    actionDialog.setMessage( message );
    actionDialog.writeToTerminal( information );
    actionDialog.exec();
}


void
KernelCommon::removeKernel( const QModelIndex& index )
{
    QStringList modules = qvariant_cast<QStringList>( index.data( KernelModel::InstalledModulesRole ) );
    QString kernel = qvariant_cast<QString>( index.data( KernelModel::PackageRole ) );
    QString version = qvariant_cast<QString>( index.data( KernelModel::VersionRole ) );
        QString kernelh = qvariant_cast<QString>( index.data( KernelModel::PackageRole ) );
    QString headers = kernelh.append("-headers");
    QStringList packageList;
    packageList << kernel << modules << headers;


    QString title = QString( tr( "Remove Linux %1" ) ).arg( version );
    QString message = QString( tr( "The Linux %1 packages will be removed. \nWould you like to continue?" ) ).arg( version );
    QString information = QString();
    foreach ( const QString p, packageList )
    {
        information.append( p );
        information.append( "\n" );
    }

    QStringList arguments;
    arguments << "--noconfirm" << "--noprogressbar" << "-R" << packageList;
    QVariantMap args;
    args["arguments"] = arguments;
    KAuth::Action installAction( QLatin1String( "org.garuda.msm.kernel.remove" ) );
    installAction.setHelperId( QLatin1String( "org.garuda.msm.kernel" ) );
    installAction.setArguments( args );
    installAction.setTimeout( std::numeric_limits<int>::max() );

    ActionDialog actionDialog;
    actionDialog.setInstallAction( installAction );
    actionDialog.setWindowTitle( title );
    actionDialog.setMessage( message );
    actionDialog.writeToTerminal( information );
    actionDialog.exec();
}


void
KernelCommon::showChangelog( const QModelIndex& index )
{
    QString package = qvariant_cast<QString>( index.data( KernelModel::PackageRole ) );
    QString majorVersion = qvariant_cast<QString>( index.data( KernelModel::MajorVersionRole ) );
    QString minorVersion = qvariant_cast<QString>( index.data( KernelModel::MinorVersionRole ) );
    QString title = QString( tr( "Linux %1.%2 changelog" ) ).arg( majorVersion, minorVersion );
    KernelInfoDialog kernelInfoDialog;
    kernelInfoDialog.setWindowTitle( title );
    kernelInfoDialog.setPackage( package );
    kernelInfoDialog.exec();
}
