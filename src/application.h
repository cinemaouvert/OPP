
/**
 * This class is used on mac platform to
 * get the file opened with a double clic.
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#if (QT_VERSION >= 0x050000) // Qt version 5 and above
#   include <QApplication>
#else // until version 5
#   include <QtGui/QApplication>
#endif

#include <QDebug>
#include <QTranslator>
#include <QSettings>
#include <QLibraryInfo>
#include <QDir>
#include <QCoreApplication>
#include <QTextCodec>
#include <QString>
#include <QFileOpenEvent>
#include <QMessageBox>

#include "mainwindow.h"
#include "customeventfilter.h"
#include "config.h"
#include "utils.h"

class Application : public QApplication
{
    Q_OBJECT
public:
    explicit Application(int &argc, char **argv);
    ~Application();

protected:
    bool event(QEvent *);

private:
    MainWindow* _win;
    QTranslator* _translator;
};

#endif // APPLICATION_H
