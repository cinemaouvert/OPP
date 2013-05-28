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
    src/application.h

SOURCES += src/media.cpp \
    src/playlist.cpp \
    src/playback.cpp \
    src/mediasettings.cpp \
    src/automator.cpp \
    src/schedule.cpp \
    src/project.cpp \
    src/playlistplayer.cpp \
    src/mediaplayer.cpp \
    src/application.cpp

mac:LIBVLC_LIB_DIR = "/Applications/VLC.app/Contents/MacOS/lib"
mac:LIBVLC_INCLUDE_DIR = "/Applications/VLC.app/Contents/MacOS/include"

mac:LIBS += -L$$LIBVLC_LIB_DIR -lvlc -lvlccore -framework Cocoa
mac:INCLUDEPATH += $$LIBVLC_INCLUDE_DIR
mac:QMAKE_CXXFLAGS+=-x objective-c++

unix:LIBS += -lvlc -lvlccore

windows:LIBS += -L"C:\Program Files (x86)\VideoLAN\VLC\sdk\lib" -llibvlc -llibvlccore
windows:INCLUDEPATH += "C:\Program Files (x86)\VideoLAN\VLC\sdk\include"
