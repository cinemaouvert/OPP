#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QDebug>
#include <QTranslator>
#include <QSettings>
#include <QLibraryInfo>

#include "mainwindow.h"
#include "media.h"

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
    {
        /*Check if OS language is available, if not English is set as default language*/
        QString locale = QLocale::system().name().section('_', 0, 0);
        if(locale=="fr")
            settings.setValue("lang","fr");
        else
            settings.setValue("lang","en");
    }

    /*Translation file*/
    QString translationFile = "opp_";
    translationFile+=settings.value("lang").toString();

    QTranslator translator;
    translator.load(translationFile);
    a.installTranslator(&translator);

    QTranslator translator2;
    translator2.load(QString("qt_") + settings.value("lang").toString(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&translator2);


    /* for save and load */
    qRegisterMetaType<MediaListModel*>("MediaListModel*");
    qRegisterMetaType< QList<Media> >("QList<Media>");
    qRegisterMetaTypeStreamOperators< QList<Media> >("QList<Media>");


    MainWindow win;
    win.show();

    return a.exec();
}
