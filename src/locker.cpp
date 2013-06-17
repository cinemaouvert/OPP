/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Baptiste Rozière <bapt.roziere@gmail.com>
 *          Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
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
#include "passformdialog.h"

#include <QWidget>
#include <QDebug>

Locker::Locker(QList<QWidget*> widgets, QObject *parent) :
    QObject(parent),
    _widgets(widgets)
{
    _passDialog = new passformdialog(this);
    this->setPasswordEnable(false);
}

bool Locker::getAutoLock(){
    return _autoLock;
}

void Locker::setAutoLock(bool lock){
    _autoLock = lock;
    if (lock == false){
        _timer.stop();
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
        _timer.start(time);
    }
}

void Locker::lock()
{
    foreach(QWidget *widget, _widgets) {
        widget->setEnabled(false);
    }
}

void Locker::unlock()
{
    if (this->passwordEnable()) {
        _passDialog->show();
        _passDialog->raise();
        _passDialog->activateWindow();
    } else {
        foreach(QWidget *widget, _widgets) {
            widget->setEnabled(true);
        }
    }
}

void Locker::toggle(bool checked)
{
    checked ? lock() : unlock();

    emit toggled(checked);
}

QList<QWidget*> Locker::getWidgets()
{
    return _widgets;
}
