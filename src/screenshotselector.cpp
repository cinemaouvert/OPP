/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Thibaud Lamarche <lamarchethibaud@hotmail.com>
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

#include "screenshotselector.h"
#include "ui_screenshotselector.h"
#include "utils.h"
#include "mainwindow.h"

#include <QDir>

ScreenshotSelector::ScreenshotSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScreenshotSelector)
{
    ui->setupUi(this);

    vlc = libvlc_new(0, NULL);
    vlcp = libvlc_media_player_new(vlc);
}

ScreenshotSelector::~ScreenshotSelector()
{
    libvlc_release(vlc);
    libvlc_media_player_release(vlcp);

    delete ui;
}

void ScreenshotSelector::setMedia(Media *media){
    this->_media = media;

    ui->startLabel->setText("00:00:00:000");
    uint duration = this->_media->duration();

    if(duration == 0)
        duration = 100;

    QTime durationTime = msecToQTime(duration);
    ui->endLabel->setText(durationTime.toString("hh:mm:ss:zzz"));
    ui->seekSlider->setMinimum(1);
    ui->seekSlider->setMaximum(duration - 1);
    ui->seekSlider->setPageStep(ui->stepInput->value());
    ui->seekSlider->setValue(1);

    _currentWId = ui->videoWidget->request();

    if (_currentWId) {
        #if defined(Q_OS_WIN)
                libvlc_media_player_set_hwnd(vlcp, (void *)_currentWId);
        #elif defined(Q_OS_MAC)
                libvlc_media_player_set_nsobject(vlcp, (void *)_currentWId);
        #elif defined(Q_OS_UNIX)
                libvlc_media_player_set_xwindow(vlcp, _currentWId);
        #endif
    }


    libvlc_media_player_set_media(vlcp, _media->core());
    libvlc_media_add_option(_media->core(),":noaudio");

    libvlc_media_player_play(vlcp);
    wait(750);
    libvlc_media_player_pause(vlcp);
}

void ScreenshotSelector::on_buttonBox_accepted()
{
    QString screenPath = qApp->applicationDirPath() + "/screenshot/";
    screenPath = screenPath.replace("/",QDir::separator());
    screenPath +=  _media->getLocation().replace(QDir::separator(),"_").remove(":");
    screenPath += ".png";

    unsigned* width = new unsigned(0);
    unsigned* height = new unsigned(0);
    libvlc_video_get_size(vlcp, 0, width, height);

    // reduce the size to a usable size
    unsigned denom = *width / (unsigned)((MainWindow*)parent())->screenBefore()->width();
    if(denom >= 1){
        *width = *width/denom;
        *height = *height/denom;
    }
    libvlc_video_take_snapshot(vlcp, 0, screenPath.toStdString().c_str(), *width, *height);

    delete width;
    delete height;

    ((MainWindow*) this->parent())->updateCurrentScreenshot();
    close();
}

void ScreenshotSelector::on_buttonBox_rejected()
{
    close();
}

void ScreenshotSelector::closeEvent (QCloseEvent *event){
    close();
}

void ScreenshotSelector::close(){
    libvlc_media_player_stop(vlcp);
}

void ScreenshotSelector::setScreenshot(int position){
    libvlc_media_player_set_time(vlcp,position);
}


void ScreenshotSelector::on_seekSlider_sliderReleased()
{
    int position = ui->seekSlider->value();
    setScreenshot(position);
    QTime durationTime = msecToQTime(position);
    ui->startLabel->setText(durationTime.toString("hh:mm:ss:zzz"));
}


void ScreenshotSelector::on_pushButton_clicked()
{
    ui->seekSlider->setValue(ui->seekSlider->value()-ui->seekSlider->pageStep());
    on_seekSlider_sliderReleased();
}

void ScreenshotSelector::on_pushButton_2_clicked()
{
    ui->seekSlider->setValue(ui->seekSlider->value()+ui->seekSlider->pageStep());
    on_seekSlider_sliderReleased();
}



void ScreenshotSelector::on_stepInput_valueChanged(int arg1)
{
    ui->seekSlider->setPageStep(arg1);
}
