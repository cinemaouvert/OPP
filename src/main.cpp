#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QDebug>
#include <QTranslator>

#include "mainwindow.h"
#include "media.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*
    QTranslator translator;
    translator.load("opp_fr");
    a.installTranslator(&translator);
    */
    /* for save and load */
    qRegisterMetaType<MediaListModel*>("MediaListModel*");
    qRegisterMetaType< QList<Media> >("QList<Media>");
    qRegisterMetaTypeStreamOperators< QList<Media> >("QList<Media>");


    MainWindow win;
    win.show();

    return a.exec();
}
