/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Baptiste Rozière <bapt.roziere@gmail.com>
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

#include "passformdialog.h"
#include "ui_passformdialog.h"
#include "locker.h"


passformdialog::passformdialog(Locker *locker, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::passformdialog)
{
    ui->setupUi(this);
    _locker = locker;
}

passformdialog::~passformdialog()
{
    delete ui;
}

QString passformdialog::getPass()
{
    return _lePass;
}

void passformdialog::on_pushButton_clicked()
{
    //enable or disable widgets
    if(this->getPass() == _locker->getPassword())
    {
        _locker->unlock();
        this->close();
    }

}

void passformdialog::on_enterPass_editingFinished()
{
    _lePass = ui->enterPass->text();
}
