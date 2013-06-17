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

#ifndef VIDEOTRACK_H
#define VIDEOTRACK_H

#include <QSize>

#include "track.h"

/**
 * @brief Manage video track information
 */
class VideoTrack : public Track
{
    Q_OBJECT
public:
    explicit VideoTrack(libvlc_media_track_info_t *vlcTrackInfo, QObject *parent = 0);
    explicit VideoTrack(QObject *parent = 0);

    /**
     * @brief Get the video size
     * @return The video size (width and height)
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    QSize size() const;

    /**
     * @brief Get the video width
     * @return The video width
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    uint width() const;

    /**
     * @brief Get the video height
     * @return The video height
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    uint height() const;
};

#endif // VIDEOTRACK_H
