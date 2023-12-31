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

#include "TimeDateCommon.h"
#include "TimeZoneDialog.h"
#include "TimeDatePage.h"
#include "ui_PageTimeDate.h"

#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtCore/QTimeZone>
#include <QtCore/QDateTime>
#include <QtCore/QTimer>

TimeDatePage::TimeDatePage( QWidget* parent ) :
    PageWidget( parent ),
    ui( new Ui::PageTimeDate ),
    m_timeDateService( new TimeDateService ),
    m_timeFieldsTimer ( new QTimer ( this ) )
{
    ui->setupUi( this );
    setTitle( TimeDateCommon::getTitle() );
    QPixmap pix=QIcon::fromTheme("preferences-system-time").pixmap(48), QIcon(":/images/resources/timedate.png");
    setIcon( pix );
    setShowApplyButton( true );
    setName( TimeDateCommon::getName() );

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
        this -> setApplyEnabled( this, true );
    } );

    connect( ui->timeZonePushButton, &QPushButton::clicked,
             [this] ()
    {
        QString newTimeZone = TimeDateCommon::showTimeZoneSelector( m_timeZone );
        if ( !newTimeZone.isEmpty() )
        {
            m_timeZone = newTimeZone;
            TimeDateCommon::updateUi( ui, m_timeDateService, m_isTimeEdited, m_isDateEdited, m_timeZone );
            this -> setApplyEnabled( this, true );
        }
    } );

    connect( ui->timeEdit, &QTimeEdit::timeChanged,
             [this] ()
    {
        m_isTimeEdited = true;
        this -> setApplyEnabled( this, true );
    } );

    connect( ui->dateEdit, &QTimeEdit::dateChanged,
             [this] ()
    {
        m_isDateEdited = true;
        this -> setApplyEnabled( this, true );
    } );

    connect( ui->isRtcLocalCheckBox, &QCheckBox::toggled,
             [this] ()
    {
        this -> setApplyEnabled( this, true );
    } );
}


TimeDatePage::~TimeDatePage()
{
    delete ui;
    delete m_timeDateService;
    delete m_timeFieldsTimer;
}


void
TimeDatePage::load()
{
    m_isTimeEdited = false;
    m_isDateEdited = false;
    m_timeZone = m_timeDateService->timeZone();
    m_timeFieldsTimer->start( 1000 );
    QApplication::restoreOverrideCursor();
    TimeDateCommon::updateUi( ui, m_timeDateService, m_isTimeEdited, m_isDateEdited, m_timeZone );
}


void
TimeDatePage::save()
{
    TimeDateCommon::save( ui, m_timeDateService, m_isTimeEdited, m_isDateEdited, m_timeZone );
    m_isTimeEdited = false;
    m_isDateEdited = false;
    this -> setApplyEnabled( this, false );
}
