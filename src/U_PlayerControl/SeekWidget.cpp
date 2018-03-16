/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
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

#include <QTime>
#include <QTimer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QSlider>

#include <vlc/vlc.h>

#include "playback.h"
#include "MediaPlayer.h"
#include "SeekWidget.h"

SeekWidget::SeekWidget(QWidget *parent)
    : QWidget(parent),
      _playlistPlayer(0),
      _mediaPlayer(0)
{
    initSeekWidget();

    connect(_seek, SIGNAL(valueChanged(int)), this, SLOT(updateTime(int)));
}

SeekWidget::~SeekWidget()
{
    delete _labelElapsed;
    delete _seek;
    delete _labelRemaining;
}

void SeekWidget::initSeekWidget()
{
    _autoHide = false;

    // ELAPSED TIME
    _labelElapsed = new QLabel(this);
    _labelElapsed->setText("00:00:00");
    _labelElapsed->setAlignment(Qt::AlignTop);
    _labelElapsed->setContentsMargins(QMargins(0,0,0,0));

    // REMAINING TIME
    _labelRemaining = new QLabel(this);
    _labelRemaining->setText("- 00:00:00");
    _labelRemaining->setAlignment(Qt::AlignRight|Qt::AlignTop);
    _labelRemaining->setContentsMargins(QMargins(0,0,0,0));

    // SEEK BAR
    _seek = new FlipBar(this);

    // LAYOUT
    QGridLayout *layout = new QGridLayout();
    layout->addWidget(_seek, 2, 1, 1, -1);
    layout->addWidget(_labelElapsed, 1, 1);
    layout->addWidget(_labelRemaining, 1, 2);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);
    layout->setContentsMargins(0, 15, 0, 4);
    setLayout(layout);

    setStyleSheet(QString(
        "QLabel{"
        "   font-size: 13px;"
        "}"
    ));
}

void SeekWidget::wheelEvent(QWheelEvent *event)
{
    event->ignore();
    initSeekWidget();

    if (!_mediaPlayer)
        return;

    if (event->delta() > 0)
        _mediaPlayer->setCurrentTime(_mediaPlayer->currentTime() + _mediaPlayer->currentLength() * 0.01);
    else
        _mediaPlayer->setCurrentTime(_mediaPlayer->currentTime() - _mediaPlayer->currentLength() * 0.01);
}


void SeekWidget::setAutoHide(const bool &autoHide)
{
    _autoHide = autoHide;

    setVisible(!_autoHide);
}

void SeekWidget::setPlaylistPlayer(PlaylistPlayer *player)
{

    disconnection();

    _playlistPlayer = player;
    _mediaPlayer = _playlistPlayer->mediaPlayer();
    _seek->setPlaylistPlayer(_playlistPlayer);

    connection();
}

void SeekWidget::connection(){
    if(_mediaPlayer){
        connect(_mediaPlayer, SIGNAL(lengthChanged(int)), this, SLOT(updateLength(int)));
        connect(_mediaPlayer, SIGNAL(timeChanged(int)), this, SLOT(updateTime(int)));
        connect(_mediaPlayer, SIGNAL(end()), this, SLOT(end()));
        connect(_mediaPlayer, SIGNAL(stopped()), this, SLOT(end()));

    }
}

void SeekWidget::disconnection(){
    if (_mediaPlayer) {
        disconnect(_mediaPlayer, SIGNAL(lengthChanged(int)), this, SLOT(updateLength(int)));
        disconnect(_mediaPlayer, SIGNAL(timeChanged(int)), this, SLOT(updateTime(int)));
        disconnect(_mediaPlayer, SIGNAL(end()), this, SLOT(end()));
        disconnect(_mediaPlayer, SIGNAL(stopped()), this, SLOT(end()));
    }
}

void SeekWidget::end()
{
    QTime time = QTime(0,0,0,0);
    QString elapsed = "hh:mm:ss";
    QString remaining = "- hh:mm:ss";

    _labelElapsed->setText(time.toString(elapsed));
    _labelRemaining->setText(time.toString(remaining));
    _seek->setMaximum(1);
    _seek->setValue(0);
}

void SeekWidget::updateTime(const int &time)
{
    /** REGLE LE PROBLEME DE TIME CODE */
    if(_mediaPlayer->isStopped())
        return;

    /** Update the current time */
    QTime currentTime = QTime(0,0,0,0).addMSecs(time);

    QString display = "hh:mm:ss";
    _labelElapsed->setText(currentTime.toString(display));
    _seek->setValue(time);

    /** Update the remaining time */
    int t = _mediaPlayer->currentPlayback()->media()->duration() - time + _mediaPlayer->currentPlayback()->mediaSettings()->inMark();
    QTime remainingTime = QTime(0,0,0,0).addMSecs(t);
     _mediaPlayer->currentPlayback()->media()->setRemainingTime(remainingTime);
    display = "- hh:mm:ss";

    _labelRemaining->setText(remainingTime.toString(display));
}

void SeekWidget::updateLength(const int &time){
    int t = _mediaPlayer->currentPlayback()->media()->duration() - time + _mediaPlayer->currentPlayback()->mediaSettings()->inMark();
    QTime remainingTime = QTime(0,0,0,0).addMSecs(t);

    QString display = "- hh:mm:ss";
    _mediaPlayer->currentPlayback()->media()->setRemainingTime(remainingTime);

    _labelRemaining->setText(remainingTime.toString(display));

    /** set the maximum time */
    if (!time) {
        _seek->setMaximum(1);
        setVisible(!_autoHide);
    } else {
        _seek->setMaximum(time);
        setVisible(true);
    }
}
