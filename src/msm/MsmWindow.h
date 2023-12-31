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

#ifndef MSMWINDOW_H
#define MSMWINDOW_H

#include "KernelPage.h"
#include "UsersPage.h"
#include "KeyboardPage.h"
#include "LanguagePackagesPage.h"
#include "TimeDatePage.h"
#include "PageWidget.h"

#include <QApplication>
#include <QMainWindow>
#include <QString>
#include <QListWidget>
#include <QPixmap>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QListWidgetItem>
#include <QDesktopWidget>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QMessageBox>


namespace Ui
{
class MsmWindow;
}


class MsmWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MsmWindow( QWidget* parent = 0 );
    ~MsmWindow();
    void readPositionSettings();
    void loadModule( QString moduleName );

private:
    class ListWidgetItem : public QListWidgetItem
    {
    public:
        ListWidgetItem( QListWidget* parent ) : QListWidgetItem( parent )
        {
            page = NULL;
        }

        PageWidget* page;
    };

    Ui::MsmWindow* ui;
    KernelPage pageKernel;
    UsersPage pageUsers;
    KeyboardPage pageKeyboard;
    LanguagePackagesPage pageLanguagePackages;
    TimeDatePage pageTimeDate;

    void addPageWidget( PageWidget& page );
    void closeEvent( QCloseEvent* );
    void writePositionSettings();

protected slots:
    void listWidget_itemActivated( QListWidgetItem* );
    void buttonShowAllSettings_clicked();
    void setApplyEnabled( PageWidget* page, bool enabled );
    void buttonApply_clicked();
    void closePageRequested( PageWidget* page );

};


#endif // MSMWINDOW_H
