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

#ifndef USERSAUTHHELPER_H
#define USERSAUTHHELPER_H

#include <KAuth>

using namespace KAuth;

class UsersAuthHelper : public QObject
{
    Q_OBJECT

public Q_SLOTS:
    ActionReply add( const QVariantMap& args );
    ActionReply remove( const QVariantMap& args );
    ActionReply changepassword( const QVariantMap& args );
    ActionReply changeaccounttype( const QVariantMap& args );
    ActionReply changeimage( const QVariantMap& args );
};

#endif // USERSAUTHHELPER_H
