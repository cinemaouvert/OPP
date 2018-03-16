/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Thomas Berthomé <thoberthome@laposte.net>
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

#include "loggersingleton.h"
#include "iostream"

#include <QFileDialog>
#include <QDebug>
#include <QApplication>
#include <QDateTime>

LoggerSingleton* LoggerSingleton::_single = NULL;
QPointer<QLabel> LoggerSingleton::_textEdit = NULL;
QMutex LoggerSingleton::_mutex;

LoggerSingleton *LoggerSingleton::getInstance()
{
    if(!_single)
    {
        _mutex.lock();
        if (!_single)
            _single = new LoggerSingleton();
        _mutex.unlock();
    }
    return _single;
}

void LoggerSingleton::destroyInstance()
{
    if(_single)
    {
        _mutex.lock();
        delete _single;
        _single = NULL;
       _mutex.unlock();
    }
}

void LoggerSingleton::setTextEdit(QLabel *textEdit)
{
    _textEdit = textEdit;
}

void LoggerSingleton::writeMessage(const QString &message)
{
    QString dt = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    QString txt = QString("[%1]\n").arg(dt);

    txt += message;

    QFile outFile(QDir(qApp->applicationDirPath()).path() + "/" + "opp.log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);

    QTextStream textStream(&outFile);
    textStream << txt << endl;

    if(_textEdit)
    {
        _mutex.lock();
        _textEdit->setText(txt+"\n"+_textEdit->text());
        _mutex.unlock();
    }
}
