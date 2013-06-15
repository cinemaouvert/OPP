#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QDebug>
#include <QTranslator>
#include <QSettings>

#include "mainwindow.h"
#include "media.h"

/*
 * TO FIX
 *
 *     ref 0000001 - move codes into PlaylistListManager.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*Settings initialization*/
    QSettings settings("opp", "opp");

    if(!settings.contains("vlcPath"))
    #if defined(Q_OS_WIN)
        settings.setValue("vlcPath", "path/to/VLC/for/Windows");
    #elif defined(Q_OS_MAC)
        settings.setValue("vlcPath", "path/to/VLC/for/Mac");
    #elif defined(Q_OS_UNIX)
        settings.setValue("vlcPath", "path/to/VLC/for/Unix");
    #endif

    if(!settings.contains("testPatternPath"))
    #if defined(Q_OS_WIN)
        settings.setValue("testPatternPath", "path/to/test/pattern/for/Windows");
    #elif defined(Q_OS_MAC)
        settings.setValue("testPatternPath", "path/to/test/pattern/for/Mac");
    #elif defined(Q_OS_UNIX)
        settings.setValue("testPatternPath", "path/to/test/pattern/for/Unix");
    #endif

    if(!settings.contains("intertitlePath"))
    #if defined(Q_OS_WIN)
        settings.setValue("intertitlePath", "path/to/intertitle/for/Windows");
    #elif defined(Q_OS_MAC)
        settings.setValue("intertitlePath", "path/to/intertitle/for/Mac");
    #elif defined(Q_OS_UNIX)
        settings.setValue("intertitlePath", "path/to/intertitle/for/Unix");
    #endif

    if(!settings.contains("moviesPath"))
    #if defined(Q_OS_WIN)
        settings.setValue("moviesPath", "path/to/movies/for/Windows");
    #elif defined(Q_OS_MAC)
        settings.setValue("moviesPath", "path/to/movies/for/Mac");
    #elif defined(Q_OS_UNIX)
        settings.setValue("moviesPath", "path/to/movies/for/Unix");
    #endif

    if(!settings.contains("lang"))
        settings.setValue("lang","en");

    /*Translation file*/
    QString translationFile = "opp_";
    translationFile+=settings.value("lang").toString();

    QTranslator translator;
    translator.load(translationFile);
    a.installTranslator(&translator);

    /* for save and load */
    qRegisterMetaType<MediaListModel*>("MediaListModel*");
    qRegisterMetaType< QList<Media> >("QList<Media>");
    qRegisterMetaTypeStreamOperators< QList<Media> >("QList<Media>");


    MainWindow win;
    win.show();

    return a.exec();
}
