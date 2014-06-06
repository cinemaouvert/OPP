/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Thibaud Lamarche <lamarchethibaud@hotmail.com>
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

#ifndef SCREENSHOTSELECTOR_H
#define SCREENSHOTSELECTOR_H

#include <QDialog>
#include "media.h"
#include <vlc/vlc.h>


namespace Ui {
class ScreenshotSelector;
}

class ScreenshotSelector : public QDialog
{
    Q_OBJECT
    
public:
    explicit ScreenshotSelector(QWidget *parent = 0);
    ~ScreenshotSelector();

    /**
      * @brief used to set the media
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    void setMedia(Media *media);
    
private slots:
    /**
      * @brief Save the screenshot selected
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    void on_buttonBox_accepted();

    /**
      * @brief Close the window
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    void on_buttonBox_rejected();

    /**
      * @brief Update the start time with the slider value
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    void on_seekSlider_sliderReleased();

    /**
      * @brief Apply the step on the slider
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    void on_pushButton_clicked();

    /**
      * @brief Apply the step on the slider
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    void on_pushButton_2_clicked();

    /**
      * @brief Set the step on the slider
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    void on_stepInput_valueChanged(int arg1);

private:
    /**
      * @brief ui The ui
      *
      */
    Ui::ScreenshotSelector *ui;

    /**
      * @brief vlc The libvlc instance
      *
      */
    libvlc_instance_t *vlc;

    /**
      * @brief vlcp The liblvc media player
      *
      */
    libvlc_media_player_t *vlcp;

    /**
      * @brief _currentWId The current WId
      *
      */
    WId _currentWId;

    /**
      * @brief _media The media
      *
      */
    Media *_media;

    /**
      * @brief Stop the libvlc media player
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    void close();

    /**
      * @brief Close the window
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    void closeEvent (QCloseEvent *event);

    /**
      * @brief Set time in the libvlc media player
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    void setScreenshot(int position);


};

#endif // SCREENSHOTSELECTOR_H
