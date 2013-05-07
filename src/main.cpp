#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QDebug>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow win;

    win.show();

    return a.exec();
}
