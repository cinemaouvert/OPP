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

#ifndef PLAYERTIMELINEWIDGET_H
#define PLAYERTIMELINEWIDGET_H

#include <QWidget>
#include <QTime>

namespace Ui {
class PlayerTimelineWidget;
}
class Media;
class MediaPlayer;

class PlayerTimelineWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PlayerTimelineWidget(QWidget *parent = 0);
    ~PlayerTimelineWidget();

    void setMediaPlayer(MediaPlayer *media);

    void setNextMedia(Media *media);
    void setFutureMedia(Media *media);

private:
    Ui::PlayerTimelineWidget *ui;
    bool _paused;
    bool _lockAutoUpdate;

    MediaPlayer *_mediaPlayer;

    Media *_nextMedia;
    Media *_futurMedia;
};

#endif // PLAYERTIMELINEWIDGET_H
