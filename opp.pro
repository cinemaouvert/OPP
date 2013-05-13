######################################################################
# OPP - Qt project file
######################################################################

TEMPLATE = app
TARGET = opp
QT += xml
DEPENDPATH += ./src
INCLUDEPATH += ./src

release:DESTDIR = build/release
debug:DESTDIR = build/debug

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.rcc
UI_DIR = $$DESTDIR/.ui

HEADERS += src/vlcwidgetvideo.h \
    src/vlc/Video.h \
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
    src/vlc/VideoDelegate.h \
    src/playertimelinewidget.h \
    src/mainwindow.h \
    src/binwidget.h \
    src/resumedetailswidget.h

SOURCES += src/vlcwidgetvideo.cpp \
    src/vlc/Video.cpp \
    src/vlc/MetaManager.cpp \
    src/vlc/MediaPlayer.cpp \
    src/vlc/MediaListPlayer.cpp \
    src/vlc/MediaList.cpp \
    src/vlc/Media.cpp \
    src/vlc/Instance.cpp \
    src/vlc/Error.cpp \
    src/vlc/Enums.cpp \
    src/vlc/Common.cpp \
    src/vlc/Audio.cpp \
    src/playertimelinewidget.cpp \
    src/mainwindow.cpp \
    src/main.cpp \
    src/binwidget.cpp \
    src/resumedetailswidget.cpp

FORMS += src/playertimelinewidget.ui \
    src/mainwindow.ui \
    src/binwidget.ui \
    src/resumedetailswidget.ui

mac:LIBVLC_LIB_DIR = "/Applications/VLC.app/Contents/MacOS/lib"
mac:LIBVLC_INCLUDE_DIR = "/Applications/VLC.app/Contents/MacOS/include"

mac:LIBS += -L$$LIBVLC_LIB_DIR -lvlc -lvlccore -framework Cocoa
mac:INCLUDEPATH += $$LIBVLC_INCLUDE_DIR
mac:QMAKE_CXXFLAGS+=-x objective-c++

unix:LIBS += -lvlc -lvlccore
