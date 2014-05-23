/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
 *
 * Open Projection Program is an initiative of Catalogue Ouvert du Cinéma.
 * The software was developed by four students of University of Poitiers
 * as school project.
 *
 * Open Projection Program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Open Projection Program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Open Projection Program. If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************************/

#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QDebug>
#include <QTranslator>
#include <QSettings>
#include <QLibraryInfo>
#include <QDir>
#include <QCoreApplication>

#include <QMessageBox>

#include "mainwindow.h"
#include "media.h"
#include "customeventfilter.h"
#include "config.h"

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
        settings.setValue("vlcPath", "C:\\Program Files (x86)\\VideoLAN\\VLC\\vlc.exe");
    #elif defined(Q_OS_MAC)
        settings.setValue("vlcPath", "/Applications/VLC.app");
    #elif defined(Q_OS_UNIX)
        settings.setValue("vlcPath", "/usr/bin/vlc");
    #endif

    QString currentDir = QCoreApplication::applicationDirPath();

    if(!settings.contains("testPatternPath"))
    #if defined(Q_OS_WIN)
        settings.setValue("testPatternPath", currentDir+"\\test_pattern.jpg");
    #elif defined(Q_OS_MAC)
        settings.setValue("testPatternPath", currentDir+"/test_pattern.jpg");
    #elif defined(Q_OS_UNIX)
        settings.setValue("testPatternPath", currentDir+"/test_pattern.jpg");
    #endif


    if(!settings.contains("intertitlePath"))
    #if defined(Q_OS_WIN)
        settings.setValue("intertitlePath", currentDir+"\\intertitle.jpg");
    #elif defined(Q_OS_MAC)
        settings.setValue("intertitlePath", currentDir+"/intertitle.jpg");
    #elif defined(Q_OS_UNIX)
        settings.setValue("intertitlePath", currentDir+"/intertitle.jpg");
    #endif

    if(!settings.contains("moviesPath"))
        settings.setValue("moviesPath", QDir::homePath ());

    if(!settings.contains("updatePath"))
        settings.setValue("updatePath", config_opp::URL);

    if(!settings.contains("lang"))
    {
        /*Check if OS language is available, if not English is set as default language*/
        QString locale = QLocale::system().name().section('_', 0, 0);
        if(locale=="fr")
            settings.setValue("lang","fr");
        else
            settings.setValue("lang","en");
    }

    //TODO TEST TRANSLATION FILES
    /*Translation file*/
    QString translationFile = "opp_";
    translationFile+=settings.value("lang").toString();
    //translationFile+="fr";

    QTranslator translator;
    translator.load(translationFile);
    a.installTranslator(&translator);

    QTranslator translator2;
    translator2.load(QString("qt_") + settings.value("lang").toString(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    //translator2.load(QString("qt_fr"), QLibraryInfo::location(QLibraryInfo::TranslationsPath));

    a.installTranslator(&translator2);



    MainWindow win;
    a.installEventFilter(new CustomEventFilter(win.getLocker(),&a));

    win.show();

    return a.exec();
}
