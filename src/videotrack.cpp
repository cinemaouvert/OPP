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

#include "videotrack.h"

VideoTrack::VideoTrack(libvlc_media_track_info_t *vlcTrackInfo, QObject *parent) :
    Track(vlcTrackInfo, parent)
{
}

VideoTrack::VideoTrack(QObject *parent) :
    Track(parent)
{
}

QSize VideoTrack::size() const
{
    return QSize(_vlcTrackInfo.u.video.i_width, _vlcTrackInfo.u.video.i_height);
}

uint VideoTrack::width() const
{
    return _vlcTrackInfo.u.video.i_width;
}

uint VideoTrack::height() const
{
    return _vlcTrackInfo.u.video.i_height;
}
