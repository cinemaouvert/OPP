/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
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

#ifndef PLAYBACK_H
#define PLAYBACK_H

#include <QObject>
#include "media.h"
#include "mediasettings.h"

/**
 * @brief Manage playback by associating a MediaSettings and a Media instance
 */
class Playback : public QObject
{
    Q_OBJECT
public:
    explicit Playback(Media *media, QObject *parent = 0);

    virtual ~Playback();

    /**
     * @brief Get media
     * @return The associated media
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline Media* media() const { return _media; }

    /**
     * @brief Get media settings
     * @return The current media settings
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline MediaSettings* mediaSettings() const { return _mediaSettings; }
    
private:

    /**
     * @brief The associated media
     */
    Media *_media;

    /**
     * @brief The media settings
     */
    MediaSettings *_mediaSettings;
    
};

#endif // PLAYBACK_H
