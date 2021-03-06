###################################################################################
# OPP - Qt project file
###################################################################################
# This file is part of Open Projection Program (OPP).
#
# Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
#
# Authors: Florian Mhun <florian.mhun@gmail.com>
#          Cyril Naud <futuramath@gmail.com>
#
# Open Projection Program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Open Projection Program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Open Projection Program. If not, see <http://www.gnu.org/licenses/>.
###################################################################################
#
#To create the application
TEMPLATE = app
TARGET = opp

#Specific to MAC to open the .opp files by default
QMAKE_INFO_PLIST += Info.plist

##To create the lib for plugins
#TEMPLATE = lib
#CONFIG += staticlib
#TARGET = GeneratedLib/opp

# Qt version 4 and 5
QT += xml
QT += network
QT += webkit

# from Qt version 5
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    QT += printsupport
    QT += webkitwidgets
}

DEPENDPATH += ./src
INCLUDEPATH += ./src

include(src/CORE.pri)
include(src/C_MediaPlayer/C_MediaPlayer.pri)
include(src/C_Playlist/C_Playlist.pri)
include(src/UI.pri)
include(src/U_PlayerControl/U_PlayerControl.pri)
include(src/U_PlaylistHandler/U_PlaylistHandler.pri)



RESOURCES += images.qrc
TRANSLATIONS = opp_fr.ts
