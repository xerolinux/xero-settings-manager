/*
 *  This file is part of XeroLinux Settings Manager.
 *
 *  Roland Singer <roland@manjaro.org>
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

#ifndef LANGUAGEPACKAGESMODULE_H
#define LANGUAGEPACKAGESMODULE_H

#include <KCModule>

namespace Ui
{
class PageLanguagePackages;
}


class LanguagePackagesModule : public KCModule
{
    Q_OBJECT

public:
    /**
     * Constructor.
     *
     * @param parent Parent widget of the module
     * @param args Arguments for the module
     */
    explicit LanguagePackagesModule( QWidget* parent, const QVariantList& args = QVariantList() );
    /**
     * Destructor.
     */
    ~LanguagePackagesModule();

    /**
     * Overloading the KCModule load() function.
     */
    void load();

    /**
     * Overloading the KCModule save() function.
     */
    void save();

    /**
     * Overloading the KCModule defaults() function.
     */
    void defaults();



private:
    Ui::PageLanguagePackages* ui;
};

#endif // LANGUAGEPACKAGESMODULE_H
