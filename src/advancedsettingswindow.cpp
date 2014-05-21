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

#include "advancedsettingswindow.h"
#include "ui_advancedsettingswindow.h"
#include "playback.h"
#include "mainwindow.h"
#include "utils.h"

#include <QDebug>

OCPM_Plugin::OCPM_Plugin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdvancedSettingsWindow),
    _playback(0)
{
    ui->setupUi(this);
    connect(ui->timeEdit_inMark, SIGNAL(timeChanged(QTime)), this, SLOT(updateLength()));
    connect(ui->timeEdit_outMark, SIGNAL(timeChanged(QTime)), this, SLOT(updateLength()));

    ui->imageDurationTimeEdit->setCurrentSectionIndex(2);
}

OCPM_Plugin::~OCPM_Plugin()
{
    delete ui;
}

void OCPM_Plugin::setPlayback(Playback* playback)
{
    _playback=playback;
    /*Movie title*/
    ui->label_title->setText(_playback->media()->name());

    /*Setting in and out mark*/
    QTime timeIn = msecToQTime(_playback->mediaSettings()->inMark());
    ui->timeEdit_inMark->setTime(timeIn);
    QTime timeOut = msecToQTime(_playback->mediaSettings()->outMark());
    ui->timeEdit_inMark->setMaximumTime(msecToQTime(_playback->media()->getOriginalDuration()));
    ui->timeEdit_outMark->setMaximumTime(msecToQTime(_playback->media()->getOriginalDuration()));

    ui->timeEdit_outMark->setTime(timeOut);

    ui->timeEdit_outMark->setCurrentSectionIndex(1);
    ui->timeEdit_inMark->setCurrentSectionIndex(1);



    /*Original length*/
    QTime original =  msecToQTime(_playback->media()->duration());
    ui->label_originalLengthValue->setText(original.toString("hh:mm:ss"));

    /*Modified length*/
    updateLength();

    ui->comboBox_testPattern->setCurrentIndex(_playback->mediaSettings()->testPattern());


    /*Fill table*/
    while (ui->tableWidget_information->rowCount() > 0)
    {
        ui->tableWidget_information->removeRow(0);
    }
    ui->tableWidget_information->insertRow(0);
    QTableWidgetItem* itemTitle = new QTableWidgetItem( tr("Title") );
    ui->tableWidget_information->setItem( 0, 0, itemTitle );
    itemTitle = new QTableWidgetItem(_playback->media()->name());
    ui->tableWidget_information->setItem( 0, 1, itemTitle);
    int line=1;
    QTableWidgetItem* itemTrack;
    foreach(VideoTrack videoTrack, _playback->media()->videoTracks())
    {
        int row = ui->tableWidget_information->rowCount();
        ui->tableWidget_information->insertRow(row);
        itemTrack = new QTableWidgetItem(QString(tr("Track %1 (Video)")).arg(QString::number(videoTrack.trackId())));
        ui->tableWidget_information->setItem(line, 0, itemTrack);
        itemTrack = new QTableWidgetItem(videoTrack.codecDescription());
        ui->tableWidget_information->setItem(line, 1, itemTrack);

        line++;
    }
    foreach(AudioTrack audioTrack, _playback->media()->audioTracks())
    {
        int row = ui->tableWidget_information->rowCount();
        ui->tableWidget_information->insertRow(row);
        itemTrack = new QTableWidgetItem(QString(tr("Track %1 (Audio)")).arg(QString::number(audioTrack.trackId())));
        ui->tableWidget_information->setItem(line, 0, itemTrack);
        itemTrack = new QTableWidgetItem(audioTrack.codecDescription());
        ui->tableWidget_information->setItem(line, 1, itemTrack);

        line++;
    }
    foreach(Track subtitlesTrack, _playback->media()->subtitlesTracks())
    {
        int row = ui->tableWidget_information->rowCount();
        ui->tableWidget_information->insertRow(row);
        itemTrack = new QTableWidgetItem(QString(tr("Track %1 (Subtitles)")).arg(QString::number(subtitlesTrack.trackId())));
        ui->tableWidget_information->setItem(line, 0, itemTrack);
        itemTrack = new QTableWidgetItem(subtitlesTrack.codecDescription());
        ui->tableWidget_information->setItem(line, 1, itemTrack);

        line++;
    }

    if(_playback->media()->isImage()){
        ui->imageDurationLabel->setVisible(true);
        ui->imageDurationTimeEdit->setVisible(true);
        ui->timeEdit_inMark->setVisible(false);
        ui->timeEdit_outMark->setVisible(false);
        ui->label_inMark->setVisible(false);
        ui->label_outMark->setVisible(false);
        ui->imageDurationTimeEdit->setTime(timeOut);

    }else{
        ui->imageDurationLabel->setVisible(false);
        ui->imageDurationTimeEdit->setVisible(false);
        ui->timeEdit_inMark->setVisible(true);
        ui->timeEdit_outMark->setVisible(true);
        ui->label_inMark->setVisible(true);
        ui->label_outMark->setVisible(true);
    }

}

void OCPM_Plugin::on_buttonBox_OKCancel_accepted()
{
    /*Test pattern*/
    int index = ui->comboBox_testPattern->currentIndex();
    if (index == -1)
        return;
    _playback->mediaSettings()->setTestPattern(index==0);

    /*In mark and out mark*/
    _playback->mediaSettings()->setInMark(qTimeToMsec(ui->timeEdit_inMark->time()));
    uint timeIn = _playback->mediaSettings()->inMark();
    char* optionIn = (QString(":start-time=") + QString::number(timeIn / 1000)).toLocal8Bit().data();
    libvlc_media_add_option(_playback->media()->core(),optionIn);

    _playback->mediaSettings()->setOutMark(qTimeToMsec(ui->timeEdit_outMark->time()));
    uint timeOut = _playback->mediaSettings()->outMark();
    char* optionOut = (QString(":stop-time=") + QString::number(timeOut / 1000)).toLocal8Bit().data();
    libvlc_media_add_option(_playback->media()->core(),optionOut);


    uint diff;
    if(_playback->media()->isImage()){
        int secOut = ui->imageDurationTimeEdit->time().second()+60*ui->imageDurationTimeEdit->time().minute()+3600*ui->imageDurationTimeEdit->time().hour();
        diff = 1000*(secOut);
        _playback->media()->setImageTime(QString::number(diff/1000));
    }else{

        _playback->mediaSettings()->setInMark(timeIn);
        _playback->mediaSettings()->setOutMark(timeOut);

        diff = timeOut - timeIn;
    }
    QString duration;
    if(diff != 0){
        duration = QString::number(diff);
        _playback->media()->setDuration(duration);
    }else{
        duration = QString::number(timeOut);
        _playback->media()->setDuration(duration);
    }
    this->hide();
}

void OCPM_Plugin::on_buttonBox_OKCancel_rejected()
{
    this->hide();
}

void OCPM_Plugin::updateLength()
{
    int secIn = ui->timeEdit_inMark->time().second()+60*ui->timeEdit_inMark->time().minute()+3600*ui->timeEdit_inMark->time().hour();
    int secOut = ui->timeEdit_outMark->time().second()+60*ui->timeEdit_outMark->time().minute()+3600*ui->timeEdit_outMark->time().hour();
    int diff = 1000*(secOut-secIn);
    QTime modified =  msecToQTime(diff);
    ui->label_modifiedLengthValue->setText(modified.toString("hh:mm:ss"));
}

void OCPM_Plugin::on_imageDurationTimeEdit_timeChanged(const QTime &date)
{
    int secOut = ui->imageDurationTimeEdit->time().second()+60*ui->imageDurationTimeEdit->time().minute()+3600*ui->imageDurationTimeEdit->time().hour();
    int diff = 1000*(secOut);
    QTime modified =  msecToQTime(diff);
    ui->label_modifiedLengthValue->setText(modified.toString("hh:mm:ss"));
}
