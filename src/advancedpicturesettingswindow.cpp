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

#include "advancedpicturesettingswindow.h"
#include "ui_advancedpicturesettingswindow.h"
#include <QDebug>

#include "playback.h"

#include "mainwindow.h"
#include "mediaplayer.h"

AdvancedPictureSettingsWindow::AdvancedPictureSettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdvancedPictureSettingsWindow),
    _playback(0)
{
    ui->setupUi(this);

    ui->comboBox_deinterlace->addItems(MediaSettings::deinterlacingValues());
}

AdvancedPictureSettingsWindow::~AdvancedPictureSettingsWindow()
{
    delete ui;
}

void AdvancedPictureSettingsWindow::on_comboBox_deinterlace_currentIndexChanged(int index)
{
    //_mediaPlayer->setCurrentDeinterlacing((Deinterlacing) index);
}

void AdvancedPictureSettingsWindow::setPlayback(Playback* playback)
{
    _playback=playback;
    ui->comboBox_deinterlace->setCurrentIndex(_playback->mediaSettings()->deinterlacing());
    ui->spinBox_up->setValue(_playback->mediaSettings()->cropTop());
    ui->spinBox_left->setValue(_playback->mediaSettings()->cropLeft());
    ui->spinBox_right->setValue(_playback->mediaSettings()->cropRight());
    ui->spinBox_down->setValue(_playback->mediaSettings()->cropBot());
}

void AdvancedPictureSettingsWindow::on_buttonBox_OKCancel_accepted()
{
    /*Setting deinterlace*/
    int indexDeinterlacing = ui->comboBox_deinterlace->currentIndex();
    if (indexDeinterlacing == -1)
        return;
    Deinterlacing deinterlacing = (Deinterlacing)indexDeinterlacing;
    _playback->mediaSettings()->setDeinterlacing(deinterlacing);

    _playback->mediaSettings()->setCrop(ui->spinBox_up->value(),
                                        ui->spinBox_left->value(),
                                        ui->spinBox_right->value(),
                                        ui->spinBox_down->value()
                                        );

    this->hide();
}

void AdvancedPictureSettingsWindow::on_buttonBox_OKCancel_rejected()
{
    this->hide();
}

void AdvancedPictureSettingsWindow::on_spinBox_up_valueChanged(int arg1)
{
    if(ui->checkBox_upDownSync->isChecked()){
        ui->spinBox_down->setValue(arg1);
    }
}

void AdvancedPictureSettingsWindow::on_spinBox_down_valueChanged(int arg1)
{
    if(ui->checkBox_upDownSync->isChecked()){
        ui->spinBox_up->setValue(arg1);
    }
}

void AdvancedPictureSettingsWindow::on_spinBox_right_valueChanged(int arg1)
{
    if(ui->checkBox_leftRightSync->isChecked()){
        ui->spinBox_left->setValue(arg1);
    }
}

void AdvancedPictureSettingsWindow::on_spinBox_left_valueChanged(int arg1)
{
    if(ui->checkBox_leftRightSync->isChecked()){
        ui->spinBox_right->setValue(arg1);
    }
}
