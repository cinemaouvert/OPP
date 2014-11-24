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

#ifndef VLCINSTANCE_H
#define VLCINSTANCE_H

#include <QObject>

struct libvlc_instance_t;

/**
 * @brief Manage the libvlc instance used by the software
 */
class VLCApplication
{
public:
    VLCApplication();
    ~VLCApplication();

    /**
     * @brief Initialize the vlc instance from list of arguments
     * @param args The list of arguments
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void initVlcInstanceFromArgs(const QStringList &args);

    /**
     * @brief Get libvlc instance
     * @return The libvlc instance
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline libvlc_instance_t* vlcInstance() const { return _vlcInstance; }

private:

    /**
     * @brief _vlcInstance The libvlc instance
     */
    libvlc_instance_t *_vlcInstance;

    /**
     * @brief Free libvlc instance
     *
     * @author Denis Saunier <saunier.denis.86@gmail.com>
     */
    void closeLibvlc();

    /**
     * @brief get color in vlc format
     * @param index of combox subtitle color
     * @return string of color
     * @author Denis Saunier <saunier.denis.86@gmail.com>
     */
    QString subtitleColor(int index);
};

#endif // VLCINSTANCE_H
