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



#include <QObject>

namespace config_opp {
    const QString URL = "http://localhost/";
    const QString VERSION = "v0.5-dev";

    #if defined(Q_OS_MAC)
        const QString LINK_UPDATE = "macos/latest/";
        const QString NAME_INSTALLER = "setup_opp.exe";
    #elif defined(Q_OS_WIN)
        const QString LINK_UPDATE = "windows/latest/";
        const QString NAME_INSTALLER = "setup_opp.exe";
    #elif defined(Q_OS_UNIX)
        const QString LINK_UPDATE = "unix/latest/";
        const QString NAME_INSTALLER = "opp.deb";
    #endif
}




#endif // CONFIG_H
