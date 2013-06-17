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

#include "track.h"

#include <vlc/plugins/vlc_fourcc.h>
#include <vlc/plugins/vlc_common.h>
#include <vlc/plugins/vlc_es.h>

#include <QDebug>

Track::Track(libvlc_media_track_info_t *vlcTrackInfo, QObject *parent) :
    QObject(parent),
    _vlcTrackInfo(*vlcTrackInfo)
{
}

Track::Track(QObject *parent) :
    QObject(parent)
{
    _vlcTrackInfo.i_id = -1;
}

Track::Track(const Track &track)
{
    _vlcTrackInfo = track._vlcTrackInfo;
}

Track::~Track()
{
}

Track & Track::operator=(const Track &track)
{
    if (this != &track) {
        _vlcTrackInfo = track._vlcTrackInfo;
    }

    return *this;
}

bool Track::operator==(const Track &track)
{
    return _vlcTrackInfo.i_id == track._vlcTrackInfo.i_id;
}

QString Track::codecDescription() const
{
    return QString( vlc_fourcc_GetDescription(UNKNOWN_ES, _vlcTrackInfo.i_codec) );
}
