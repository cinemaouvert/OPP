HEADERS += src/media.h

SOURCES += src/media.cpp

mac:LIBVLC_LIB_DIR = "/Applications/VLC.app/Contents/MacOS/lib"
mac:LIBVLC_INCLUDE_DIR = "/Applications/VLC.app/Contents/MacOS/include"

mac:LIBS += -L$$LIBVLC_LIB_DIR -lvlc -lvlccore -framework Cocoa
mac:INCLUDEPATH += $$LIBVLC_INCLUDE_DIR
mac:QMAKE_CXXFLAGS+=-x objective-c++

unix:LIBS += -lvlc -lvlccore
