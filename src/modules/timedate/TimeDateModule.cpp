/*
 *  This file is part of XeroLinux Settings Manager.
 *
 *  Ramon Buldó <ramon@manjaro.org>
 *  Kacper Piwiński
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

#include "MsmCommon.h"
#include "TimeDateCommon.h"
#include "TimeZoneDialog.h"
#include "TimeDateModule.h"
#include "ui_PageTimeDate.h"

#include <KAboutData>
#include <KPluginFactory>

#include <QtCore/QStringList>
#include <QtCore/QTimeZone>
#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtCore/QTranslator>

K_PLUGIN_FACTORY( MsmTimeDateFactory,
                  registerPlugin<TimeDateModule>( TimeDateCommon::getName() ); )

TimeDateModule::TimeDateModule( QWidget* parent, const QVariantList& args ) :
    KCModule( parent, args ),
    ui( new Ui::PageTimeDate ),
    m_timeDateService( new TimeDateService ),
    m_timeFieldsTimer ( new QTimer ( this ) )
{
    Q_INIT_RESOURCE( translations );
    QTranslator* appTranslator = new QTranslator;
    appTranslator->load( ":/translations/msm_" + QLocale::system().name() );
    qApp->installTranslator( appTranslator );

    KAboutData* aboutData = new KAboutData( TimeDateCommon::getName(),
                                            TimeDateCommon::getTitle(),
                                            PROJECT_VERSION,
                                            TimeDateCommon::getDescription(),
                                            KAboutLicense::LicenseKey::GPL_V3,
                                            "(c) 2014 - 2019 XeroLinux Settings Manager developers" );
    aboutData->addAuthor( "Ramon Buldó",
                          QStringLiteral( "ramon@manjaro.org" ) );
    aboutData->setCustomAuthorText( QString(), MsmCommon::getBugReportLink() );
    setAboutData( aboutData );
    setButtons( KCModule::Default | KCModule::Apply );

    ui->setupUi( this );

    connect( m_timeFieldsTimer, &QTimer::timeout,
             [this] ()
    {
        TimeDateCommon::updateTimeFields( ui, m_timeDateService, m_isTimeEdited, m_isDateEdited );
    } );

    connect( ui->isNtpEnabledCheckBox, &QCheckBox::toggled,
             [this] ( bool checked )
    {
        ui->timeEdit->setEnabled( !checked );
        ui->dateEdit->setEnabled( !checked );
        emit changed();
    } );

    connect( ui->timeZonePushButton, &QPushButton::clicked,
             [this] ()
    {
        QString newTimeZone = TimeDateCommon::showTimeZoneSelector( m_timeZone );
        if ( !newTimeZone.isEmpty() )
        {
            m_timeZone = newTimeZone;
            TimeDateCommon::updateUi( ui, m_timeDateService, m_isTimeEdited, m_isDateEdited, m_timeZone );
            emit changed();
        }
    } );

    connect( ui->timeEdit, &QTimeEdit::timeChanged,
             [this] ()
    {
        m_isTimeEdited = true;
        emit changed();
    } );

    connect( ui->dateEdit, &QTimeEdit::dateChanged,
             [this] ()
    {
        m_isDateEdited = true;
        emit changed();
    } );

    connect( ui->isRtcLocalCheckBox, &QCheckBox::toggled,
             [this] ()
    {
        emit changed();
    } );
}


TimeDateModule::~TimeDateModule()
{
    delete ui;
    delete m_timeDateService;
    delete m_timeFieldsTimer;
}


void
TimeDateModule::load()
{
    m_isTimeEdited = false;
    m_isDateEdited = false;
    m_timeZone = m_timeDateService->timeZone();
    m_timeFieldsTimer->start( 1000 );
    TimeDateCommon::updateUi( ui, m_timeDateService, m_isTimeEdited, m_isDateEdited, m_timeZone );
}


void
TimeDateModule::save()
{
    TimeDateCommon::save( ui, m_timeDateService, m_isTimeEdited, m_isDateEdited, m_timeZone );
    m_isTimeEdited = false;
    m_isDateEdited = false;
}


void
TimeDateModule::defaults()
{
    load();
}

#include "TimeDateModule.moc"
