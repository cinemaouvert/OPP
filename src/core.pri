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
    src/videotrack.h

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
unix:LIBS += -lvlc -lvlccore
unix:INCLUDEPATH += /usr/include/vlc/plugins
windows:LIBS += -L"C:\Program Files (x86)\VideoLAN\VLC\sdk\lib" -llibvlc -llibvlccore

mac:INCLUDEPATH += ./include
mac:INCLUDEPATH += "/Applications/VLC.app/Contents/MacOS/include"

mac:LIBS += -framework Cocoa
mac:QMAKE_CXXFLAGS+=-x objective-c++

# install deps dll on windows
