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

HEADERS += src/media.h \
    src/playlist.h \
    src/playback.h \
    src/mediasettings.h \
    src/schedule.h \
    src/playlistplayer.h \
    src/mediaplayer.h \
    src/videoview.h \
    src/application.h \
    src/track.h \
    src/audiotrack.h \
    src/videotrack.h \
    src/plugins.h

SOURCES += src/media.cpp \
    src/playlist.cpp \
    src/playback.cpp \
    src/mediasettings.cpp \
    src/schedule.cpp \
    src/playlistplayer.cpp \
    src/mediaplayer.cpp \
    src/application.cpp \
    src/track.cpp \
    src/audiotrack.cpp \
    src/videotrack.cpp

# vlc library and headers
mac:LIBS += -L"/Applications/VLC.app/Contents/MacOS/lib"
mac:INCLUDEPATH += "./include"
mac:INCLUDEPATH += "./include/vlc/plugins"
mac:INCLUDEPATH += "/Applications/VLC.app/Contents/MacOS/include"


unix:LIBS += -lvlc -lvlccore
unix:!macx:LIBS += -lX11
unix:!macx:INCLUDEPATH += /usr/include/vlc/plugins


windows:LIBS += -L"C:\Program Files (x86)\VideoLAN\VLC\sdk\lib" -llibvlc -llibvlccore
windows:INCLUDEPATH += "C:\Program Files (x86)\VideoLAN\VLC\sdk\include"
windows:INCLUDEPATH += "C:\Program Files (x86)\VideoLAN\VLC\sdk\include\vlc"
windows:INCLUDEPATH += "C:\Program Files (x86)\VideoLAN\VLC\sdk\include\vlc\plugins"
#windows:INCLUDEPATH += ./windows/include   #USE THAT LINE WITH VISUAL STUDIO

mac:LIBS += -framework Cocoa
mac:QMAKE_CXXFLAGS+=-x objective-c++
