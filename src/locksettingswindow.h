/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Baptiste Rozière <bapt.roziere@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
 *
 * Open Projection Program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Open Projection Program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Open Projection Program. If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************************/

#ifndef LOCKSETTINGSWINDOW_H
#define LOCKSETTINGSWINDOW_H

#include <QDialog>

namespace Ui {
class LockSettingsWindow;
}
class Locker;

class LockSettingsWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit LockSettingsWindow(Locker *locker, QWidget *parent = 0);
    ~LockSettingsWindow();
    void setLocker();
private slots:
    void on_radioButton_passwordOn_clicked();

    void on_radioButton_passwordOff_clicked();

    void on_radioButton_autoLockOn_clicked();

    void on_radioButton_autoLockOff_clicked();

    void on_lineEdit_password_editingFinished();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::LockSettingsWindow *ui;
    Locker *_locker;
};

#endif // LOCKSETTINGSWINDOW_H
