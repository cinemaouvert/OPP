######################################################################
# OPP - Qt project file
######################################################################

TEMPLATE = app
TARGET = opp
QT += xml
DEPENDPATH += ./src
INCLUDEPATH += ./src

include(src/core.pri)
include(src/ui.pri)

RESOURCES += images.qrc
