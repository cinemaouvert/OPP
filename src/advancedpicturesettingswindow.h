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

#ifndef ADVANCEDPICTURESETTINGSWINDOW_H
#define ADVANCEDPICTURESETTINGSWINDOW_H

#include <QDialog>

namespace Ui {
class AdvancedPictureSettingsWindow;
}

class Playback;
class AdvancedPictureSettingsWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit AdvancedPictureSettingsWindow(QWidget *parent = 0);
    ~AdvancedPictureSettingsWindow();

    /**
     * @brief Set a playback to the window
     * @param playback The playback to use
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void setPlayback(Playback* playback);
    
private slots:
    /**
     * @brief Set a new value for Deinterlacing
     * @param index The new value for Deinterlacing
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_comboBox_deinterlace_currentIndexChanged(int index);

    /**
     * @brief Validate and apply changes
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_buttonBox_OKCancel_accepted();

    /**
     * @brief Cancel changes
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_buttonBox_OKCancel_rejected();

private:
    /**
     * @brief ui The UI
     */
    Ui::AdvancedPictureSettingsWindow *ui;

    /**
     * @brief _playback The playback modified by the window
     */
    Playback* _playback;
};

#endif // ADVANCEDPICTURESETTINGSWINDOW_H
