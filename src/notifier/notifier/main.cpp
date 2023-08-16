/*
 *  This file is part of Garuda Settings Manager.
 *
 *  Ramon Buldó <ramon@manjaro.org>
 *  Kacper Piwiński
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

#include "NotifierApp.h"
#include "Notifier.h"
#include "kdsingleapplicationguard/kdsingleapplicationguard.h"

#include <QtWidgets/QApplication>
#include <QtCore/QTranslator>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QCommandLineParser>
#include <QDebug>

int main( int argc, char* argv[] )
{
    NotifierApp app( argc, argv );

    Q_INIT_RESOURCE( language_packages );
    Q_INIT_RESOURCE( translations );

    QCommandLineParser parser;
    parser.setApplicationDescription( app.applicationName() );
    parser.addHelpOption();

    QCommandLineOption settingsOption( QStringList() << "s" << "settings",
                                    "Show msm notifier options" );
    parser.addOption( settingsOption );
    parser.process( app );

    QTranslator appTranslator;
    appTranslator.load( ":/translations/msm_" + QLocale::system().name() );
    app.installTranslator( &appTranslator );

    // Workaround to enable to call the dialog to change settings with a command argument
    // because if the application is running and tray icon is hidden there is no way to change settings.
    if ( parser.isSet( settingsOption ) )
    {
        NotifierSettingsDialog* m_settingsDialog = new NotifierSettingsDialog( NULL );
        m_settingsDialog->setAttribute( Qt::WidgetAttribute::WA_DeleteOnClose, true );
        return m_settingsDialog->exec();
    }

    // ensure only one tray icon application is runnung
    KDSingleApplicationGuard guard( KDSingleApplicationGuard::AutoKillOtherInstances );

    if ( guard.isPrimaryInstance() )
    {
        app.init();
        Notifier notifier( &app );
        return app.exec();
    }
    else
    {
        qDebug() << "MSM Notifier is already running, shutting down.";
    }

    return 0;
}
