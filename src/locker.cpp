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

#include"mainwindow.h"

Locker::Locker(QList<QWidget*> widgets, QObject *parent) :
    QObject(parent),
    _autoLock(false),
    _widgets(widgets)
{
    this->setPasswordEnable(false);
    _lock = false;
    _time = -1;
}

Locker::~Locker()
{

}

bool Locker::getAutoLock(){
    return _autoLock;
}

void Locker::setAutoLock(bool lock){
    _autoLock = lock;
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
        _time = time;
    }
}

void Locker::autoLock() {
    if (getAutoLock() == true) {
        lock();
    }
}

void Locker::lock()
{
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
                ((MainWindow *)this->parent())->updateSettings();
                _lock = false;
        }
        else {
            emit toggled(true);
        }
    } else {
        foreach(QWidget *widget, _widgets) {
            widget->setEnabled(true);
        }
        ((MainWindow *)this->parent())->updateSettings();
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

bool Locker::isLock(){
    return _lock;
}

int Locker::getTime(){
    return _time;
}
