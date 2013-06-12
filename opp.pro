######################################################################
# OPP - Qt project file
######################################################################

TEMPLATE = app
TARGET = opp
QT += xml
DEPENDPATH += ./src
INCLUDEPATH += ./src
QMAKESPEC = linux-g++-32

include(src/core.pri)
include(src/ui.pri)

RESOURCES += images.qrc
