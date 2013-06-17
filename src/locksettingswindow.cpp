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

#include "locksettingswindow.h"
#include "ui_locksettingswindow.h"

#include "locker.h"

LockSettingsWindow::LockSettingsWindow(Locker *locker, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LockSettingsWindow),
    _locker(locker)
{
    ui->setupUi(this);
    //_locker->setPasswordEnable(false);
}

LockSettingsWindow::~LockSettingsWindow()
{
    delete ui;
}

void LockSettingsWindow::on_radioButton_passwordOn_clicked()
{
    //_locker->setPasswordEnable(true);
     ui->lineEdit_password->setEnabled(true);
}

void LockSettingsWindow::on_radioButton_passwordOff_clicked()
{
    //_locker->setPasswordEnable(false);
    ui->lineEdit_password->setEnabled(false);
}

void LockSettingsWindow::on_radioButton_autoLockOn_clicked()
{
    //_locker->setAutoLock(true);
}

void LockSettingsWindow::on_radioButton_autoLockOff_clicked()
{
   // _locker->setAutoLock(false);
}

void LockSettingsWindow::on_lineEdit_password_editingFinished()
{
    //_locker->setPassword(ui->lineEdit_password->text());
}

void LockSettingsWindow::on_buttonBox_accepted()
{
    /*Password*/
    _locker->setPasswordEnable(ui->radioButton_passwordOn->isChecked());
    if(ui->radioButton_passwordOn->isChecked())
        _locker->setPassword(ui->lineEdit_password->text());

    /*Automatic lock*/
    _locker->setAutoLock(ui->radioButton_autoLockOn->isChecked());
    _locker->setAutoLockDelay(ui->spinBox_delay->value());

    this->hide();
}

void LockSettingsWindow::on_buttonBox_rejected()
{
    this->hide();
}

void LockSettingsWindow::setLocker()
{
    /*Password*/
    if(_locker->passwordEnable())
    {
        ui->radioButton_passwordOn->setChecked(true);
        ui->radioButton_passwordOff->setChecked(false);
        ui->lineEdit_password->setEnabled(true);
    }
    else
    {
        ui->radioButton_passwordOn->setChecked(false);
        ui->radioButton_passwordOff->setChecked(true);
        ui->lineEdit_password->setEnabled(false);
    }
    ui->lineEdit_password->setText(_locker->getPassword());
    /*Automatic lock*/
    if(_locker->getAutoLock())
    {
        ui->radioButton_autoLockOn->setChecked(true);
        ui->radioButton_autoLockOff->setChecked(false);
    }
    else
    {
        ui->radioButton_autoLockOn->setChecked(false);
        ui->radioButton_autoLockOff->setChecked(true);
    }
    ui->spinBox_delay->setValue(0);

}
