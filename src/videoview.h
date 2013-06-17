/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
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

#ifndef VIDEOVIEW_H
#define VIDEOVIEW_H

#include <QtCore/QObject>
#include <QtGui/qwindowdefs.h>

/**
 * @interface VideoView
 * @brief The video view interface
 */
class VideoView
{
public:
    VideoView() {}
    virtual ~VideoView() {}

    /**
     * @brief Get the window identifier in which the video should be rendered
     * @return The window identifier
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    virtual WId request() = 0;

    /**
     * @brief Release the video rendered into the window
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    virtual void release() = 0;
};

#endif // VIDEOVIEW_H
