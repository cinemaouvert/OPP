/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Baptiste Rozière <bapt.roziere@gmail.com>
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


#include <QInputDialog>

#include "locksettingswindow.h"
#include "ui_locksettingswindow.h"

#include "locker.h"

LockSettingsWindow::LockSettingsWindow(Locker *locker, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LockSettingsWindow),
    _locker(locker)
{
    ui->setupUi(this);
}

LockSettingsWindow::~LockSettingsWindow()
{
    delete ui;
}

void LockSettingsWindow::on_radioButton_passwordOn_clicked()
{
     ui->lineEdit_password->setEnabled(true);
}

void LockSettingsWindow::on_radioButton_passwordOff_clicked()
{
    if(askPassword())
    {
        ui->lineEdit_password->setEnabled(false);
    }
    else
    {
        ui->radioButton_passwordOn->setChecked(true);
        ui->radioButton_passwordOff->setChecked(false);
    }
}

void LockSettingsWindow::on_radioButton_autoLockOn_clicked()
{
    ui->spinBox_delay->setEnabled(true);
}

void LockSettingsWindow::on_radioButton_autoLockOff_clicked()
{
    ui->spinBox_delay->setEnabled(false);
}

void LockSettingsWindow::on_buttonBox_accepted()
{
    /*Password*/
    if(ui->radioButton_passwordOn->isChecked())
    {
        if(_locker->passwordEnable())
        {
            if(ui->lineEdit_password->text().compare(_locker->getPassword())!=0)
            {
                    if(askPassword())
                    {
                        _locker->setPassword(ui->lineEdit_password->text());
                    }
            }
        }
        else
        {
            _locker->setPasswordEnable(ui->radioButton_passwordOn->isChecked());
            _locker->setPassword(ui->lineEdit_password->text());
        }
    }
    else
    {
         _locker->setPasswordEnable(ui->radioButton_passwordOn->isChecked());
    }

    /*Automatic lock*/
    _locker->setAutoLock(ui->radioButton_autoLockOn->isChecked());
    _locker->setAutoLockDelay(ui->spinBox_delay->value()*1000);  /*s -> ms*/

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
        ui->spinBox_delay->setEnabled(true);
    }
    else
    {
        ui->radioButton_autoLockOn->setChecked(false);
        ui->radioButton_autoLockOff->setChecked(true);
        ui->spinBox_delay->setEnabled(false);
    }
    ui->spinBox_delay->setValue(0);

}

bool LockSettingsWindow::askPassword() {
    bool ok, res = false;
    QString text = QInputDialog::getText(NULL,
        tr("Enter password"),
        tr("Password : "),
        QLineEdit::Password,
        tr(""),
        &ok
    );
    if(ok) {
        if(text.compare(_locker->getPassword())==0) {
            res = true;
        }
    }
    return res;
}
