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

#ifndef TRACK_H
#define TRACK_H

#include "config.h"
#include <QObject>

#include <vlc/vlc.h>

/**
 * @brief Base class for track information
 */
class Track : public QObject
{
    Q_OBJECT            
public:

    /** VLC before version 2.1.0 */
    explicit Track(libvlc_media_track_info_t* vlcTrackInfo, QObject *parent = 0);

    /** VLC after version 2.1.0 */
    explicit Track(libvlc_media_track_t** vlcTrack, QObject *parent = 0);

    explicit Track(QObject *parent = 0);
    virtual ~Track();
    Track(const Track &track);
    Track & operator=(const Track &track);

    /**
     * @brief Overload the operator ==
     * @param track The track to be compared with
     * @return True if both equals, false otherwise
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    bool operator==(const Track &track);

    /**
     * @brief Get libvlc track id
     * @warning Liblvc seems to not consider this identifier but the index of the track.
     * @return The liblvc track id
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline int trackId() const { if(config_opp::LIBVLC_MAJOR <= 2 && config_opp::LIBVLC_MINOR < 1){ return _vlcTrackInfo.i_id; }else{ return _vlcTrack.i_id;  }}

    /**
     * @brief Get libvlc codec identifier
     * @return The libvlc code identifier
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline uint codec() const { if(config_opp::LIBVLC_MAJOR <= 2 && config_opp::LIBVLC_MINOR < 1){ return _vlcTrackInfo.i_codec; }else{ return _vlcTrack.i_codec; }}

    /**
     * @brief Get liblvc track type
     * @return The libvlc track type
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline libvlc_track_type_t trackType() const { if(config_opp::LIBVLC_MAJOR <= 2 && config_opp::LIBVLC_MINOR < 1){ return _vlcTrackInfo.i_type; }else{ return _vlcTrack.i_type; }}

    /**
     * @brief Get codec description
     * @return The codec description
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    QString codecDescription() const;

protected:

    /**
     * @brief The libvlc structure where track informations are stored
     */
    libvlc_media_track_info_t _vlcTrackInfo;  /** VLC before version 2.1.0 */

    libvlc_media_track_t _vlcTrack; /** VLC after version 2.1.0 */
};

#endif // TRACK_H
