/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
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

#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include <QMainWindow>

#include "videowidget.h"

class VideoWindow : public QMainWindow
{
    Q_OBJECT
public:

    /**
     * @brief Contains display mode values
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    enum DisplayMode {
        /**
         * Video onto a classical window
         */
        WINDOW = 0,

        /**
         * Fullscreen video into the 2nd display
         */
        PROJECTION = 1
    };

    explicit VideoWindow(QWidget *parent = 0,const DisplayMode &mode = WINDOW);

    ~VideoWindow();

    /**
     * @brief Returns the video widget
     * @return The video widget
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline VideoWidget* videoWidget() const { return _videoWidget; }

    /**
     * @brief Change the display mode
     * @param mode The display mode to apply
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setDisplayMode(const DisplayMode &mode);

    /**
     * @brief Move the video window
     * @param display
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void moveToDisplay(const int &display);

    /**
     * @brief Shut down the video when closing the window
     * @param event
     *
     * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
     */
    void closeEvent (QCloseEvent *event);


private:
    /**
     * @brief _videoWidget The video widget
     */
    VideoWidget *_videoWidget;

    /**
     * @brief _mode The display mode
     */
    DisplayMode _mode;
};

#endif // VIDEOWINDOW_H
