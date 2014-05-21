/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
 *          Baptiste Rozière <bapt.roziere@gmail.com>
 *          Hamza Haddar <ham.haddar@gmail.com>
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

#ifndef ADVANCEDSETTINGSWINDOW_H
#define ADVANCEDSETTINGSWINDOW_H

#include <QDialog>

namespace Ui {
class AdvancedSettingsWindow;
}
class Playback;
class OCPM_Plugin : public QDialog
{
    Q_OBJECT
    
public:
    explicit OCPM_Plugin(QWidget *parent = 0);
    ~OCPM_Plugin();
    /**
     * @brief Apply a playback to the window
     * @param playback The playback to apply
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void setPlayback(Playback* playback);

public slots:
    /**
     * @brief Update the value of the label Modified length
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void updateLength();
    
private slots:
    /**
     * @brief Save modified settings and close the window
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_buttonBox_OKCancel_accepted();

    /**
     * @brief Close the window
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_buttonBox_OKCancel_rejected();

    void on_imageDurationTimeEdit_timeChanged(const QTime &date);

private:
    /**
     * @brief ui The UI
     */
    Ui::AdvancedSettingsWindow *ui;
    /**
     * @brief _playback The playback
     */
    Playback* _playback;
};

#endif // ADVANCEDSETTINGSWINDOW_H
