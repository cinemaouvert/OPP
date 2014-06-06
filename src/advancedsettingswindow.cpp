/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
 *          Baptiste Rozière <bapt.roziere@gmail.com>
 *          Hamza Haddar <ham.haddar@gmail.com>
 *          Geoffrey Bergé <geoffrey.berge@live.fr>
 *          Thomas Berthomé <thoberthome@laposte.net>
 *          Thibaud Lamarche <lamarchethibaud@hotmail.com>
 *          Denis Saunier <saunier.denis.86@gmail.com>
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
#include <QMessageBox>
#include <QDir>

AdvancedSettings::AdvancedSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdvancedSettingsWindow),
    selectorWindow(NULL),
    _playback(0)
{
    ui->setupUi(this);
    connect(ui->timeEdit_inMark, SIGNAL(timeChanged(QTime)), this, SLOT(updateLength()));
    connect(ui->timeEdit_outMark, SIGNAL(timeChanged(QTime)), this, SLOT(updateLength()));

    ui->imageDurationTimeEdit->setCurrentSectionIndex(2);

    selectorWindow = new ScreenshotSelector(parent);
}

AdvancedSettings::~AdvancedSettings()
{
    delete ui;
}

void AdvancedSettings::setPlayback(Playback* playback)
{
    _playback=playback;
    /*Movie title*/
    ui->label_title->setText(_playback->media()->name());

    /*Setting in and out mark*/
    QTime timeIn = msecToQTime(_playback->mediaSettings()->inMark());
    QTime timeOut = msecToQTime(_playback->mediaSettings()->outMark());

    ui->timeEdit_inMark->setTime(timeIn);
    ui->timeEdit_inMark->setMaximumTime(msecToQTime(_playback->media()->getOriginalDuration()));
    ui->timeEdit_outMark->setMaximumTime(msecToQTime(_playback->media()->getOriginalDuration()));

    ui->timeEdit_outMark->setTime(timeOut);

    ui->timeEdit_outMark->setCurrentSectionIndex(1);
    ui->timeEdit_inMark->setCurrentSectionIndex(1);
    ui->timeEdit_CrossFading->setCurrentSectionIndex(2);
    ui->timeEdit_CrossFadingIN->setCurrentSectionIndex(2);
    ui->timeEdit_VideoFadeIN->setCurrentSectionIndex(2);
    ui->timeEdit_VideoFadeOut->setCurrentSectionIndex(2);

    ui->timeEdit_CrossFading->setMaximumTime(msecToQTime(_playback->media()->getOriginalDuration()));
    ui->timeEdit_CrossFadingIN->setMaximumTime(msecToQTime(_playback->media()->getOriginalDuration()));
    ui->timeEdit_VideoFadeOut->setMaximumTime(msecToQTime(_playback->media()->getOriginalDuration()));
    ui->timeEdit_VideoFadeIN->setMaximumTime(msecToQTime(_playback->media()->getOriginalDuration()));

    ui->timeEdit_CrossFading->setTime(msecToQTime(_playback->mediaSettings()->audioFadeOut()));
    ui->timeEdit_CrossFadingIN->setTime(msecToQTime(_playback->mediaSettings()->audioFadeIn()));
    ui->timeEdit_VideoFadeOut->setTime(msecToQTime(_playback->mediaSettings()->videoFadeOut()));
    ui->timeEdit_VideoFadeIN->setTime(msecToQTime(_playback->mediaSettings()->videoFadeIn()));  

    /*Original length*/
    QTime original =  msecToQTime(_playback->media()->duration());
    ui->label_originalLengthValue->setText(original.toString("hh:mm:ss"));

    /*Modified length*/
    updateLength();

    //ui->comboBox_testPattern->setCurrentIndex(_playback->mediaSettings()->testPattern());


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
        ui->label_CrossFading->setVisible(false);
        ui->timeEdit_CrossFading->setVisible(false);
        ui->label_CrossFading_2->setVisible(false);
        ui->timeEdit_CrossFadingIN->setVisible(false);
        ui->imageDurationTimeEdit->setTime(timeOut);
        ui->label_CrossFading_3->setVisible(true);
        ui->label_CrossFading_4->setVisible(true);
        ui->timeEdit_VideoFadeIN->setVisible(true);
        ui->timeEdit_VideoFadeOut->setVisible(true);


    }else{
        ui->imageDurationLabel->setVisible(false);
        ui->imageDurationTimeEdit->setVisible(false);
        ui->timeEdit_inMark->setVisible(true);
        ui->timeEdit_outMark->setVisible(true);
        ui->label_inMark->setVisible(true);
        ui->label_outMark->setVisible(true);
        ui->label_CrossFading->setVisible(true);
        ui->timeEdit_CrossFading->setVisible(true);
        ui->label_CrossFading_2->setVisible(true);
        ui->timeEdit_CrossFadingIN->setVisible(true);
        if(_playback->media()->isAudio()){
            ui->label_CrossFading_3->setVisible(false);
            ui->label_CrossFading_4->setVisible(false);
            ui->timeEdit_VideoFadeIN->setVisible(false);
            ui->timeEdit_VideoFadeOut->setVisible(false);
        }else{
            ui->label_CrossFading_3->setVisible(true);
            ui->label_CrossFading_4->setVisible(true);
            ui->timeEdit_VideoFadeIN->setVisible(true);
            ui->timeEdit_VideoFadeOut->setVisible(true);

        }
    }

    ui->changeScreenshotButton->setVisible(false);
    if(!_playback->media()->isAudio() && !_playback->media()->isImage()){
        QPixmap pixmap;
        QString path = "./screenshot/";
        path = path.replace("/",QDir::separator());
        path +=  _playback->media()->getLocation().replace(QDir::separator(),"_");
        path += ".png";
        pixmap.load((path.toStdString().c_str()));

        ui->changeScreenshotButton->setVisible(true);
        ui->label_picture->setPixmap(pixmap.scaled( QSize(400,400), Qt::KeepAspectRatio, Qt::FastTransformation));
    }else{
        ui->label_picture->clear();
    }
}

void AdvancedSettings::on_buttonBox_OKCancel_accepted()
{
    /*Test pattern*/
    //int index = ui->comboBox_testPattern->currentIndex();
    // if (index == -1)
    //    return;
    //_playback->mediaSettings()->setTestPattern(index==0);

    /*In mark and out mark*/
    uint timeIn = qTimeToMsec(ui->timeEdit_inMark->time());
    uint timeOut = qTimeToMsec(ui->timeEdit_outMark->time());


    if(_playback->media()->isImage()){
        _playback->mediaSettings()->setVideoFadeOut(qTimeToMsec(ui->timeEdit_VideoFadeOut->time()));
        _playback->mediaSettings()->setVideoFadeIn(qTimeToMsec(ui->timeEdit_VideoFadeIN->time()));

    }else if(_playback->media()->isAudio()){
        _playback->mediaSettings()->setAudioFadeOut(qTimeToMsec(ui->timeEdit_CrossFading->time()));
        _playback->mediaSettings()->setAudioFadeIn(qTimeToMsec(ui->timeEdit_CrossFadingIN->time()));
    }else{
        _playback->mediaSettings()->setAudioFadeOut(qTimeToMsec(ui->timeEdit_CrossFading->time()));
        _playback->mediaSettings()->setAudioFadeIn(qTimeToMsec(ui->timeEdit_CrossFadingIN->time()));
        _playback->mediaSettings()->setVideoFadeOut(qTimeToMsec(ui->timeEdit_VideoFadeOut->time()));
        _playback->mediaSettings()->setVideoFadeIn(qTimeToMsec(ui->timeEdit_VideoFadeIN->time()));
    }


    int diff;
    if(_playback->media()->isImage()){
        int secOut = ui->imageDurationTimeEdit->time().second()+60*ui->imageDurationTimeEdit->time().minute()+3600*ui->imageDurationTimeEdit->time().hour();
        diff = 1000*(secOut);
        if(diff > 0){
            _playback->media()->setImageTime(QString::number(diff/1000));
        }
    }else{
        diff = timeOut - timeIn;
        if(diff > 0){
            _playback->mediaSettings()->setInMark(qTimeToMsec(ui->timeEdit_inMark->time()));
            _playback->mediaSettings()->setOutMark(qTimeToMsec(ui->timeEdit_outMark->time()));


            char* optionIn = (QString(":start-time=") + QString::number(timeIn / 1000)).toLocal8Bit().data();
            libvlc_media_add_option(_playback->media()->core(),optionIn);
            char* optionOut = (QString(":stop-time=") + QString::number(timeOut / 1000)).toLocal8Bit().data();
            libvlc_media_add_option(_playback->media()->core(),optionOut);


            _playback->mediaSettings()->setInMark(timeIn);
            _playback->mediaSettings()->setOutMark(timeOut);
        }


    }
    QString duration;
    if(diff > 0){
        duration = QString::number(diff);
        _playback->media()->setDuration(duration);
    }else{
        QMessageBox::warning(NULL, tr("Negative time"), tr("Start time is lower or equals to Stop time.") , tr("Ok"));
    }



    this->hide();
}

void AdvancedSettings::on_buttonBox_OKCancel_rejected()
{
    this->hide();
}

void AdvancedSettings::updateLength()
{
    int secIn = ui->timeEdit_inMark->time().second()+60*ui->timeEdit_inMark->time().minute()+3600*ui->timeEdit_inMark->time().hour();
    int secOut = ui->timeEdit_outMark->time().second()+60*ui->timeEdit_outMark->time().minute()+3600*ui->timeEdit_outMark->time().hour();
    int diff = 1000*(secOut-secIn);
    QTime modified =  msecToQTime(diff);
    ui->label_modifiedLengthValue->setText(modified.toString("hh:mm:ss"));
}

void AdvancedSettings::on_imageDurationTimeEdit_timeChanged(const QTime &date)
{
    int secOut = ui->imageDurationTimeEdit->time().second()+60*ui->imageDurationTimeEdit->time().minute()+3600*ui->imageDurationTimeEdit->time().hour();
    int diff = 1000*(secOut);
    QTime modified =  msecToQTime(diff);
    ui->label_modifiedLengthValue->setText(modified.toString("hh:mm:ss"));
}

void AdvancedSettings::on_changeScreenshotButton_clicked()
{
    selectorWindow->show();
    selectorWindow->setMedia(_playback->media());
}
