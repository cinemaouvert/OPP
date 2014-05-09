/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Baptiste Rozière <bapt.roziere@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
 *
 * Open Projection Program is an initiative of Catalogue Ouvert du Cinéma.
 * The software was developed by four students of University of Poitiers
 * as school project.
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

    /**
     * @brief Load locker parameters into the window
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void setLocker();
private slots:
    /**
     * @brief Enable password edition
     *
     * @author Baptiste Rozière <bapt.roziere@gmail.com>
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_radioButton_passwordOn_clicked();

    /**
     * @brief Disable password edition
     *
     * @author Baptiste Rozière <bapt.roziere@gmail.com>
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_radioButton_passwordOff_clicked();

    /**
     * @brief Enable lock delay edition
     *
     * @author Baptiste Rozière <bapt.roziere@gmail.com>
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_radioButton_autoLockOn_clicked();

    /**
     * @brief Disable lock delay edition
     *
     * @author Baptiste Rozière <bapt.roziere@gmail.com>
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_radioButton_autoLockOff_clicked();

    /**
     * @brief Save modified settings and close the window
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_buttonBox_accepted();

    /**
     * @brief Close the window
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_buttonBox_rejected();

    /**
     * @brief Ask password
     *
     * @author Thomas Berthome <thoberthome@laposte.net>
     */
    bool askPassword();

private:
    /**
     * @brief ui The UI
     */
    Ui::LockSettingsWindow *ui;
    /**
     * @brief _locker The locker to modify
     */
    Locker *_locker;
};

#endif // LOCKSETTINGSWINDOW_H
