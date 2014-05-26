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

#include "seekwidget.h"

#include <QTime>
#include <QTimer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>

#include "mediaplayer.h"

SeekWidget::SeekWidget(QWidget *parent)
    : QWidget(parent),
      _vlcMediaPlayer(0)
{
    initSeekWidget();
}

SeekWidget::~SeekWidget()
{
    delete _seek;
    delete _labelElapsed;
    delete _labelFull;
}

void SeekWidget::initSeekWidget()
{
    _lock = false;
    _autoHide = false;

    _seek = new QProgressBar(this);
    _seek->setOrientation(Qt::Horizontal);
    _seek->setMaximum(1);
    _seek->setTextVisible(false);
    _seek->setMaximumHeight(15);

    _labelElapsed = new QLabel(this);
    _labelElapsed->setText("00:00:00");

    _labelFull = new QLabel(this);
    _labelFull->setText("00:00:00");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(_labelElapsed);
    layout->addWidget(_seek);
    layout->addWidget(_labelFull);
    setLayout(layout);
}

void SeekWidget::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();

    if (!_lock)
        return;

    updateEvent(event->pos());
}

void SeekWidget::mousePressEvent(QMouseEvent *event)
{
    event->ignore();

    lock();
}

void SeekWidget::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();

    updateEvent(event->pos());

    unlock();
}

void SeekWidget::wheelEvent(QWheelEvent *event)
{
    event->ignore();

    if (!_vlcMediaPlayer)
        return;

    if (event->delta() > 0)
        _vlcMediaPlayer->setCurrentTime(_vlcMediaPlayer->currentTime() + _vlcMediaPlayer->currentLength() * 0.01);
    else
        _vlcMediaPlayer->setCurrentTime(_vlcMediaPlayer->currentTime() - _vlcMediaPlayer->currentLength() * 0.01);
}

void SeekWidget::setAutoHide(const bool &autoHide)
{
    _autoHide = autoHide;

    setVisible(!_autoHide);
}

void SeekWidget::setMediaPlayer(MediaPlayer *player, MainWindow * mw)
{
    if (_vlcMediaPlayer) {
        disconnect(_vlcMediaPlayer, SIGNAL(lengthChanged(int)), this, SLOT(updateFullTime(int)));
        disconnect(_vlcMediaPlayer, SIGNAL(timeChanged(int)), this, SLOT(updateCurrentTime(int)));
        disconnect(_vlcMediaPlayer, SIGNAL(timeChanged(int)), mw, SLOT(updateBackTime(int)));
        disconnect(_vlcMediaPlayer, SIGNAL(end()), this, SLOT(end()));
        disconnect(_vlcMediaPlayer, SIGNAL(stopped()), this, SLOT(end()));
    }

    _vlcMediaPlayer = player;

    connect(_vlcMediaPlayer, SIGNAL(lengthChanged(int)), this, SLOT(updateFullTime(int)));
    connect(_vlcMediaPlayer, SIGNAL(timeChanged(int)), this, SLOT(updateCurrentTime(int)));
    connect(_vlcMediaPlayer, SIGNAL(timeChanged(int)), mw, SLOT(updateBackTime(int)));
    connect(_vlcMediaPlayer, SIGNAL(end()), this, SLOT(end()));
    connect(_vlcMediaPlayer, SIGNAL(stopped()), this, SLOT(end()));
}

void SeekWidget::end()
{
    QTime time = QTime(0,0,0,0);
    QString display = "mm:ss";

    _labelElapsed->setText(time.toString(display));
    _labelFull->setText(time.toString(display));
    _seek->setMaximum(1);
    _seek->setValue(0);
}

void SeekWidget::updateEvent(const QPoint &pos)
{
    if (!_vlcMediaPlayer)
        return;

    if (pos.x() < _seek->pos().x() || pos.x() > _seek->pos().x() + _seek->width())
        return;

    float click = pos.x() - _seek->pos().x();
    float op = _seek->maximum()/_seek->width();
    float newValue = click * op;

    _vlcMediaPlayer->setCurrentTime(newValue);
    _seek->setValue(newValue);
}

void SeekWidget::updateCurrentTime(const int &time)
{
    if (_lock)
        return;

    QTime currentTime = QTime(0,0,0,0).addMSecs(time);

    QString display = "hh:mm:ss";
    _labelElapsed->setText(currentTime.toString(display));
    _seek->setValue(time);
}


void SeekWidget::updateFullTime(const int &time)
{
    if (_lock)
        return;

    QTime fullTime = QTime(0,0,0,0).addMSecs(time);

    QString display = "hh:mm:ss";

    _labelFull->setText(fullTime.toString(display));

    if (!time) {
        _seek->setMaximum(1);
        setVisible(!_autoHide);
    } else {
        _seek->setMaximum(time);
        setVisible(true);
    }
}

void SeekWidget::lock()
{
    _lock = true;
}

void SeekWidget::unlock()
{
    _lock = false;
}
