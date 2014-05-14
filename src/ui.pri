###################################################################################
# OPP - Qt project file
###################################################################################
# This file is part of Open Projection Program (OPP).
#
# Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
#
# Authors: Florian Mhun <florian.mhun@gmail.com>
#          Cyril Naud <futuramath@gmail.com>
#          Baptiste Rozière <bapt.roziere@gmail.com>
#
# Open Projection Program is an initiative of Catalogue Ouvert du Cinéma.
# The software was developed by four students of University of Poitiers
# as school project.
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

HEADERS += src/mainwindow.h \
    src/saturationwidget.h \
    src/advancedpicturesettingswindow.h \
    src/advancedsettingswindow.h \
    src/settingswindow.h \
    src/locksettingswindow.h \
    src/medialistmodel.h \
    src/playlistmodel.h \
    src/videowidget.h \
    src/videowindow.h \
    src/mediatableview.h \
    src/playlisttableview.h \
    src/seekwidget.h \
    src/schedulelistmodel.h \
    src/utils.h \
    src/locker.h \
    src/statuswidget.h \
    src/datastorage.h \
    src/playlistlistmanager.h \
    src/aboutdialog.h \
    src/customeventfilter.h

SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/saturationwidget.cpp \
    src/advancedpicturesettingswindow.cpp \
    src/advancedsettingswindow.cpp \
    src/settingswindow.cpp \
    src/locksettingswindow.cpp \
    src/medialistmodel.cpp \
    src/playlistmodel.cpp \
    src/videowidget.cpp \
    src/videowindow.cpp \
    src/mediatableview.cpp \
    src/playlisttableview.cpp \
    src/seekwidget.cpp \
    src/schedulelistmodel.cpp \
    src/utils.cpp \
    src/locker.cpp \
    src/statuswidget.cpp \
    src/datastorage.cpp \
    src/playlistlistmanager.cpp \
    src/aboutdialog.cpp \
    src/customeventfilter.cpp

FORMS += src/mainwindow.ui \
    src/saturationwidget.ui \
    src/advancedpicturesettingswindow.ui \
    src/advancedsettingswindow.ui \
    src/settingswindow.ui \
    src/locksettingswindow.ui \
    src/statuswidget.ui \
    src/aboutdialog.ui

TRANSLATIONS = opp_fr.ts
