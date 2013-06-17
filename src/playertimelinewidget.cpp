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

#include "playertimelinewidget.h"
#include "ui_playertimelinewidget.h"

#include "media.h"
#include "mediaplayer.h"

#include <QDebug>

PlayerTimelineWidget::PlayerTimelineWidget(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::PlayerTimelineWidget),
   _mediaPlayer(NULL),
   _nextMedia(NULL),
   _futurMedia(NULL)
{
   ui->setupUi(this);
}

PlayerTimelineWidget::~PlayerTimelineWidget()
{
   delete ui;
}

void PlayerTimelineWidget::setMediaPlayer(MediaPlayer *mediaPlayer)
{
    ui->seekWidget->setMediaPlayer(mediaPlayer);
}

void PlayerTimelineWidget::setNextMedia(Media *media)
{
    _nextMedia = media;
}

void PlayerTimelineWidget::setFutureMedia(Media *media)
{
    _futurMedia = media;
}
