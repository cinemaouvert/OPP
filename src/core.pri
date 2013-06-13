HEADERS += src/media.h \
    src/playlist.h \
    src/playback.h \
    src/mediasettings.h \
    src/automator.h \
    src/schedule.h \
    src/project.h \
    src/playlistplayer.h \
    src/mediaplayer.h \
    src/videoview.h \
    src/application.h \
    src/global.h \
    src/track.h \
    src/audiotrack.h \
    src/videotrack.h

SOURCES += src/media.cpp \
    src/playlist.cpp \
    src/playback.cpp \
    src/mediasettings.cpp \
    src/automator.cpp \
    src/schedule.cpp \
    src/project.cpp \
    src/playlistplayer.cpp \
    src/mediaplayer.cpp \
    src/application.cpp \
    src/track.cpp \
    src/audiotrack.cpp \
    src/videotrack.cpp

# vlc library and headers
mac:LIBS += -L"/Applications/VLC.app/Contents/MacOS/lib"
unix:LIBS += -lvlc -lvlccore
windows:LIBS += -L"C:\Program Files (x86)\VideoLAN\VLC\sdk\lib" -llibvlc -llibvlccore
INCLUDEPATH += ./include

mac:LIBS += -framework Cocoa
mac:QMAKE_CXXFLAGS+=-x objective-c++
