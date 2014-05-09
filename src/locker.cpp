/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Baptiste Rozière <bapt.roziere@gmail.com>
 *          Florian Mhun <florian.mhun@gmail.com>
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

#include "locker.h"

#include <QWidget>
#include <QDebug>
#include <QInputDialog>

Locker::Locker(QList<QWidget*> widgets, QObject *parent) :
    QObject(parent),
    _widgets(widgets),
    _autoLock(false)
{
    this->setPasswordEnable(false);
    _timer = new QTimer();
    _lock = false;
}

Locker::~Locker()
{
    delete _timer;
}

bool Locker::getAutoLock(){
    return _autoLock;
}

void Locker::setAutoLock(bool lock){
    _autoLock = lock;
    if (lock == false){
        _timer->stop();
    }
}

bool Locker::passwordEnable(){
    return _password;
}

void Locker::setPasswordEnable(bool pass){
    _password = pass;
}

QString Locker::getPassword(){
    return _thePass;
}

void Locker::setPassword(QString newPass){
    if (passwordEnable() == true) {
        _thePass = newPass;
    }
}

void Locker::setAutoLockDelay(int time){
    if (getAutoLock() == true) {
        _timer->connect(_timer, SIGNAL(timeout()), this, SLOT(lock()));
        _timer->start(time);
    }
}

void Locker::lock()
{
    _timer->stop();
    foreach(QWidget *widget, _widgets) {
        widget->setEnabled(false);
    }
    _lock = true;
    emit toggled(true);
}

void Locker::unlock()
{
    if (this->passwordEnable()) {
        bool ok;
        QString text = QInputDialog::getText(NULL,
            tr("Enter password"),
            tr("Password : "),
            QLineEdit::Password,
            tr(""),
            &ok
        );
        if(ok && text.compare(_thePass)==0) {
                foreach(QWidget *widget, _widgets) {
                    widget->setEnabled(true);
                }
                _lock = false;
        }
        else {
            emit toggled(true);
        }
    } else {
        foreach(QWidget *widget, _widgets) {
            widget->setEnabled(true);
        }
        _lock = false;
    }

}

void Locker::toggle(bool checked)
{
    emit toggled(checked);
    checked ? lock() : unlock();
}

QList<QWidget*> Locker::getWidgets()
{
    return _widgets;
}

bool Locker::getLock(){
    return _lock;
}
