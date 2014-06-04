/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
 *          Baptiste Rozière <bapt.roziere@gmail.com>
 *          Hamza Haddar <ham.haddar@gmail.com>
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

#include "utils.h"
#include <qeventloop.h>
#include <qcoreapplication.h>
#include <QString>
#include <QStringList>
#include <QStringListIterator>
#include <QObject>

QTime msecToQTime(uint msecs)
{
    const int hours = msecs/(1000*60*60);
    const int minutes = (msecs-(hours*1000*60*60))/(1000*60);
    const int seconds = (msecs-(minutes*1000*60)-(hours*1000*60*60))/1000;
    const int mseconds = msecs - (minutes*1000*60) - (hours*1000*60*60) - (seconds*1000);
    return QTime(hours,minutes,seconds,mseconds);
}

int qTimeToMsec(QTime time)
{
    int hours = time.hour();
    int minutes = time.minute();
    int seconds = time.second();
    int msec = time.msec();
    return 1000*seconds+60000*minutes+3600000*hours+msec;
}

void wait(int t)
{
    QTime dieTime= QTime::currentTime().addMSecs(t);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

QString humanSize(int size)
{
    float num = (float)size;
    QStringList list;
    list << QObject::tr("KB") << QObject::tr("MB") << QObject::tr("GB") << QObject::tr("TB");

    QStringListIterator i(list);
    QString unit(QObject::tr("bytes"));

    while(num >= 1024.0 && i.hasNext())
     {
        unit = i.next();
        num /= 1024.0;
    }
    return QString().setNum(num,'f',2)+" "+unit;
}
