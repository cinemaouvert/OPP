/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Denis Saunier <saunier.denis.86@gmail.com>
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

#ifndef CONFIG_H
#define CONFIG_H

#include <vlc/vlc.h>

#include <QObject>
#include <QDebug>

/** allow to hide the deprecated declarations during debug */
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

namespace config_opp {
    /**
     * @brief Test the Qt version and the libvlc version and define the version of OPP
     * @return {QString} The current version of OPP
     * @author Yohann Vioujard <y.vioujard@gmail.com>
     */
    QString getOppVersion();

    /**
     * @brief Allow to get the libvlc version string in QString
     * @return {QString} The current version of libvlc
     * @author Yohann Vioujard <y.vioujard@gmail.com>
     */
    QString getLibVlcVersion();

    /**
     * @brief Allow to get the libvlc version major in int type
     * @return {int} The current major version of libvlc
     * @author Yohann Vioujard <y.vioujard@gmail.com>
     */
    int getLibVlcMajor();

    /**
     * @brief Allow to get the libvlc minor major in int type
     * @return {int} The current minor version of libvlc
     * @author Yohann Vioujard <y.vioujard@gmail.com>
     */
    int getLibVlcMinor();

    /**
     * @brief Allow to get the libvlc version patch in int type
     * @return {int} The current patch version of libvlc
     * @author Yohann Vioujard <y.vioujard@gmail.com>
     */
    int getLibVlcPatch();

    const QString URL = "http://cinemaouvert.fr/";
    const QString VERSION = getOppVersion();
    const QString LIBVLC_VERSION = getLibVlcVersion();
    const int LIBVLC_MAJOR = getLibVlcMajor();
    const int LIBVLC_MINOR = getLibVlcMinor();
    const int LIBVLC_PATCH = getLibVlcPatch();


    #if defined(Q_OS_MAC)
        const QString LINK_UPDATE = "macos/latest/";
        const QString NAME_INSTALLER = "oppFinal.pkg";
    #elif defined(Q_OS_WIN)
        const QString LINK_UPDATE = "windows/latest/";
        const QString NAME_INSTALLER = "setup_opp.exe";
    #elif defined(Q_OS_UNIX)
        const QString LINK_UPDATE = "unix/latest/";
        const QString NAME_INSTALLER = "OPP_1.0-stable_amd64.deb";
    #endif
}




#endif // CONFIG_H
