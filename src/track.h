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

#ifndef TRACK_H
#define TRACK_H

#include <QObject>

#include <vlc/vlc.h>

class Track : public QObject
{
    Q_OBJECT            
public:

    explicit Track(libvlc_media_track_info_t *vlcTrackInfo, QObject *parent = 0);

    explicit Track(QObject *parent = 0);

    virtual ~Track();

    /**
     * @brief Track
     * @param track
     */
    Track(const Track &track);

    /**
     * @brief operator =
     * @param track
     * @return
     */
    Track & operator=(const Track &track);

    /**
     * @brief operator ==
     * @param track
     * @return
     */
    bool operator==(const Track &track);

    /**
     * @brief trackId
     * @return
     */
    inline int trackId() const { return _vlcTrackInfo.i_id; }

    /**
     * @brief codec
     * @return
     */
    inline uint codec() const { return _vlcTrackInfo.i_codec; }

    /**
     * @brief trackType
     * @return
     */
    inline libvlc_track_type_t trackType() const { return _vlcTrackInfo.i_type; }

    /**
     * @brief codecDescription
     * @return
     */
    QString codecDescription() const;

protected:

    /**
     * @brief _vlcTrackInfo
     */
    libvlc_media_track_info_t _vlcTrackInfo;
};

#endif // TRACK_H
