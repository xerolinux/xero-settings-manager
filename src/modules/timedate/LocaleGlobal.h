/* === This file is part of Calamares - <http://github.com/calamares> ===
 *
 *   Copyright 2014, Teo Mrnjavac <teo@kde.org>
 *
 *   Originally from the XeroLinux Installation Framework
 *   by Roland Singer <roland@manjaro.org>
 *   Copyright (C) 2007 Free Software Foundation, Inc.
 *
 *   Calamares is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Calamares is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Calamares. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LOCALEGLOBAL_H
#define LOCALEGLOBAL_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QStringList>
#include <QList>
#include <QHash>
#include <QMap>
#include <QRegExp>
#include <QDebug>

#define LOCALESDIR "/usr/share/i18n/locales"
#define TZ_DATA_FILE "/usr/share/zoneinfo/zone.tab"
#define USER_IMAGES_PATH "/usr/share/pixmaps/faces"

class LocaleGlobal
{
public:
    struct Locale
    {
        QString description;
        QString locale;
    };

    struct Location
    {
        QString region;
        QString zone;
        double latitude;
        double longitude;
    };

    static void init();
    static QHash<QString, QHash<QString, QList<LocaleGlobal::Locale> > > getLocales();
    static QHash<QString, QList<LocaleGlobal::Location> > getLocations();

private:
    static QHash<QString, QHash<QString, QList<LocaleGlobal::Locale> > > locales;
    static QHash<QString, QList<LocaleGlobal::Location> > locations;

    static void initLocales();
    static void initLocations();
    static double getRightGeoLocation( QString str );
};

#endif // LOCALEGLOBAL_H
