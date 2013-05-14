HEADERS += src/vlc/Video.h \
    src/vlc/SharedExportCore.h \
    src/vlc/MetaManager.h \
    src/vlc/MediaPlayer.h \
    src/vlc/MediaListPlayer.h \
    src/vlc/MediaList.h \
    src/vlc/Media.h \
    src/vlc/Instance.h \
    src/vlc/Error.h \
    src/vlc/Enums.h \
    src/vlc/Common.h \
    src/vlc/Audio.h \
    src/vlc/VideoDelegate.h

SOURCES += src/vlc/Video.cpp \
    src/vlc/MetaManager.cpp \
    src/vlc/MediaPlayer.cpp \
    src/vlc/MediaListPlayer.cpp \
    src/vlc/MediaList.cpp \
    src/vlc/Media.cpp \
    src/vlc/Instance.cpp \
    src/vlc/Error.cpp \
    src/vlc/Enums.cpp \
    src/vlc/Common.cpp \
    src/vlc/Audio.cpp

mac:LIBVLC_LIB_DIR = "/Applications/VLC.app/Contents/MacOS/lib"
mac:LIBVLC_INCLUDE_DIR = "/Applications/VLC.app/Contents/MacOS/include"

mac:LIBS += -L$$LIBVLC_LIB_DIR -lvlc -lvlccore -framework Cocoa
mac:INCLUDEPATH += $$LIBVLC_INCLUDE_DIR
mac:QMAKE_CXXFLAGS+=-x objective-c++

unix:LIBS += -lvlc -lvlccore
