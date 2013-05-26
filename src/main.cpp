#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QDebug>
#include <QTranslator>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*
    QTranslator translator;
    translator.load("opp_fr");
    a.installTranslator(&translator);
    */
    MainWindow win;

    win.show();

    return a.exec();
}
